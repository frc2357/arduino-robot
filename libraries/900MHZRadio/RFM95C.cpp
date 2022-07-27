#include "RFM95C.h"

// interrupt handler and related code must be in RAM on ESP8266,
// according to issue #46.
#if defined(ESP8266)
#define INTERRUPT_ATTR ICACHE_RAM_ATTR
#else
#define INTERRUPT_ATTR
#endif

// #define SERIAL_DEBUG // Uncomment to recieve debug information over serial

// Interrupt vectors for the 3 Arduino interrupt pins
// Each interrupt can be handled by a different instance of RFM_95C, allowing you to have
// 2 or more LORAs per Arduino
RFM_95C *RFM_95C::_deviceForInterrupt[RFM_95C_NUM_INTERRUPTS] = {0, 0};
uint8_t RFM_95C::_interruptCount = 0; // Index into _deviceForInterrupt for next device

// These are indexed by the values of ModemConfigChoice
// Stored in flash (program) memory to save SRAM
PROGMEM static const RFM_95C::ModemConfig MODEM_CONFIG_TABLE[] =
    {
        //  1d,     1e,      26
        {0x72, 0x74, 0x04}, // Bw125Cr45Sf128 (the chip default), AGC enabled
        {0x92, 0x74, 0x04}, // Bw500Cr45Sf128, AGC enabled
        {0x48, 0x94, 0x04}, // Bw31_25Cr48Sf512, AGC enabled
        {0x78, 0xc4, 0x0c}, // Bw125Cr48Sf4096, AGC enabled

};

RFM_95C::RFM_95C(uint8_t slaveSelectPin, uint8_t interruptPin, RHGenericSPI &spi)
    : RHSPIDriver(slaveSelectPin, spi),
      _rxBufValid(0)
{
    _interruptPin = interruptPin;
    _myInterruptIndex = 0xff; // Not allocated yet
}

bool RFM_95C::init()
{
    if (!RHSPIDriver::init())
    {
#ifdef SERIAL_DEBUG
        Serial.println(F("ERROR: Failed to initialize SPI Driver."));
#endif
        return false;
    }

    // Determine the interrupt number that corresponds to the interruptPin
    int interruptNumber = digitalPinToInterrupt(_interruptPin);
    if (interruptNumber == NOT_AN_INTERRUPT)
    {
#ifdef SERIAL_DEBUG
        Serial.println(F("ERROR: Provided pin does not support interrupts."));
#endif
        return false;
    }

#ifdef RH_ATTACHINTERRUPT_TAKES_PIN_NUMBER
    interruptNumber = _interruptPin;
#endif

    // Tell the low level SPI interface we will use SPI within this interrupt
    spiUsingInterrupt(interruptNumber);

    // No way to check the device type :-(

    // Set sleep mode, so we can also set LORA mode:
    spiWrite(RFM_95C_REG_01_OP_MODE, RFM_95C_MODE_SLEEP | RFM_95C_LONG_RANGE_MODE);
    delay(10); // Wait for sleep mode to take over from say, CAD
    // Check we are in sleep mode, with LORA set
    if (spiRead(RFM_95C_REG_01_OP_MODE) != (RFM_95C_MODE_SLEEP | RFM_95C_LONG_RANGE_MODE))
    {
#ifdef SERIAL_DEBUG
        Serial.println(F("ERROR: Failed to put device in LoRa mode."));
        Serial.print(F("RFM_95C_REG_01_OP_MODE: "));
        Serial.println(spiRead(RFM_95C_REG_01_OP_MODE), HEX);
#endif
        return false; // No device present?
    }

    // Add by Adrien van den Bossche <vandenbo@univ-tlse2.fr> for Teensy
    // ARM M4 requires the below. else pin interrupt doesn't work properly.
    // On all other platforms, its innocuous, belt and braces
    pinMode(_interruptPin, INPUT);

    // Set up interrupt handler
    // Since there are a limited number of interrupt glue functions isr*() available,
    // we can only support a limited number of devices simultaneously
    // ON some devices, notably most Arduinos, the interrupt pin passed in is actuallt the
    // interrupt number. You have to figure out the interruptnumber-to-interruptpin mapping
    // yourself based on knwledge of what Arduino board you are running on.
    if (_myInterruptIndex == 0xff)
    {
        // First run, no interrupt allocated yet
        if (_interruptCount <= RFM_95C_NUM_INTERRUPTS)
            _myInterruptIndex = _interruptCount++;
        else
        {
#ifdef SERIAL_DEBUG
            Serial.println(F("ERROR: Insufficient interrupt vectors."));
#endif
            return false; // Too many devices, not enough interrupt vectors
        }
    }
    _deviceForInterrupt[_myInterruptIndex] = this;
    if (_myInterruptIndex == 0)
        attachInterrupt(interruptNumber, isr0, RISING);
    else if (_myInterruptIndex == 1)
        attachInterrupt(interruptNumber, isr1, RISING);
    else if (_myInterruptIndex == 2)
        attachInterrupt(interruptNumber, isr2, RISING);
    else
    {
#ifdef SERIAL_DEBUG
        Serial.println(F("ERROR: Failed to attach interrupt pin. Insufficient interrupt vectors."));
#endif
        return false; // Too many devices, not enough interrupt vectors
    }

    // Set up FIFO
    // We configure so that we can use the entire 256 byte FIFO for either receive
    // or transmit, but not both at the same time
    spiWrite(RFM_95C_REG_0E_FIFO_TX_BASE_ADDR, 0);
    spiWrite(RFM_95C_REG_0F_FIFO_RX_BASE_ADDR, 0);

    // Packet format is preamble + explicit-header + payload + crc
    // Explicit Header Mode
    // payload is TO + FROM + ID + FLAGS + message data
    // RX mode is implmented with RXCONTINUOUS
    // max message data length is 255 - 4 = 251 octets

    setModeIdle();

    // Set up default configuration
    // No Sync Words in LORA mode.
    setModemConfig(Bw125Cr45Sf128); // Radio default
                                    //    setModemConfig(Bw125Cr48Sf4096); // slow and reliable?
    setPreambleLength(4);           // Default is 4
    // An innocuous ISM frequency, same as RF22's
    setFrequency(434.0);
    // Lowish power
    setTxPower(13);

    return true;
}

// C++ level interrupt handler for this instance
// LORA is unusual in that it has several interrupt lines, and not a single, combined one.
// On MiniWirelessLoRa, only one of the several interrupt lines (DI0) from the RFM95 is usefuly
// connnected to the processor.
// We use this to get RxDone and TxDone interrupts
void RFM_95C::handleInterrupt()
{
    // Read the interrupt register
    uint8_t irq_flags = spiRead(RFM_95C_REG_12_IRQ_FLAGS);
    // Read the RegHopChannel register to check if CRC presence is signalled
    // in the header. If not it might be a stray (noise) packet.*
    uint8_t crc_present = spiRead(RFM_95C_REG_1C_HOP_CHANNEL);

    if (_mode == RHModeRx && ((irq_flags & (RFM_95C_RX_TIMEOUT | RFM_95C_PAYLOAD_CRC_ERROR)) | !(crc_present & RFM_95C_RX_PAYLOAD_CRC_IS_ON)))
    //    if (_mode == RHModeRx && irq_flags & (RFM_95C_RX_TIMEOUT | RFM_95C_PAYLOAD_CRC_ERROR))
    {
        _rxBad++;
        Serial.println("CRC Error");
    }
    else if (_mode == RHModeRx && irq_flags & RFM_95C_RX_DONE)
    {
        // Have received a packet
        uint8_t len = spiRead(RFM_95C_REG_13_RX_NB_BYTES);

        // Reset the fifo read ptr to the beginning of the packet
        spiWrite(RFM_95C_REG_0D_FIFO_ADDR_PTR, spiRead(RFM_95C_REG_10_FIFO_RX_CURRENT_ADDR));
        spiBurstRead(RFM_95C_REG_00_FIFO, _buf, len);
        _bufLen = len;
        spiWrite(RFM_95C_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags

        // Remember the last signal to noise ratio, LORA mode
        // Per page 111, SX1276/77/78/79 datasheet
        _lastSNR = (int8_t)spiRead(RFM_95C_REG_19_PKT_SNR_VALUE) / 4;

        // Remember the RSSI of this packet, LORA mode
        // this is according to the doc, but is it really correct?
        // weakest receiveable signals are reported RSSI at about -66
        _lastRssi = spiRead(RFM_95C_REG_1A_PKT_RSSI_VALUE);
        // Adjust the RSSI, datasheet page 87
        if (_lastSNR < 0)
            _lastRssi = _lastRssi + _lastSNR;
        else
            _lastRssi = (int)_lastRssi * 16 / 15;
        if (_usingHFport)
            _lastRssi -= 157;
        else
            _lastRssi -= 164;

        // We have received a message.
        validateRxBuf();
        if (_rxBufValid)
            setModeIdle(); // Got one
    }
    else if (_mode == RHModeTx && irq_flags & RFM_95C_TX_DONE)
    {
        _txGood++;
        setModeIdle();
    }
    else if (_mode == RHModeCad && irq_flags & RFM_95C_CAD_DONE)
    {
        _cad = irq_flags & RFM_95C_CAD_DETECTED;
        setModeIdle();
    }
    // Sigh: on some processors, for some unknown reason, doing this only once does not actually
    // clear the radio's interrupt flag. So we do it twice. Why?
    spiWrite(RFM_95C_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
    spiWrite(RFM_95C_REG_12_IRQ_FLAGS, 0xff); // Clear all IRQ flags
}

// These are low level functions that call the interrupt handler for the correct
// instance of RFM_95C.
// 3 interrupts allows us to have 3 different devices
void INTERRUPT_ATTR RFM_95C::isr0()
{
    if (_deviceForInterrupt[0])
        _deviceForInterrupt[0]->handleInterrupt();
}
void INTERRUPT_ATTR RFM_95C::isr1()
{
    if (_deviceForInterrupt[1])
        _deviceForInterrupt[1]->handleInterrupt();
}
void INTERRUPT_ATTR RFM_95C::isr2()
{
    if (_deviceForInterrupt[2])
        _deviceForInterrupt[2]->handleInterrupt();
}

// Check whether the latest received message is complete and uncorrupted
void RFM_95C::validateRxBuf()
{
    // Extract the expected length of the message
    _rxGood++;
    _rxBufValid = true;
}

bool RFM_95C::available()
{
    if (_mode == RHModeTx)
        return false;
    setModeRx();
    return _rxBufValid; // Will be set by the interrupt handler when a good message is received
}

void RFM_95C::clearRxBuf()
{
    ATOMIC_BLOCK_START;
    _rxBufValid = false;
    _bufLen = 0;
    ATOMIC_BLOCK_END;
}

bool RFM_95C::recv(uint8_t *buf, uint8_t *len)
{
    if (!available())
        return false;
    if (buf && len)
    {
        ATOMIC_BLOCK_START;
        if (*len > _bufLen - RFM_95C_HEADER_LEN)
            *len = _bufLen - RFM_95C_HEADER_LEN;
        memcpy(buf, _buf + RFM_95C_HEADER_LEN, *len);
        ATOMIC_BLOCK_END;
    }
    clearRxBuf(); // This message accepted and cleared
    return true;
}

bool RFM_95C::send(const uint8_t *data, uint8_t len)
{
    if (len > RFM_95C_MAX_MESSAGE_LEN)
        return false;

    waitPacketSent(); // Make sure we dont interrupt an outgoing message
    setModeIdle();

    if (!waitCAD())
        return false; // Check channel activity

    // Position at the beginning of the FIFO
    spiWrite(RFM_95C_REG_0D_FIFO_ADDR_PTR, 0);
    // The message data
    spiBurstWrite(RFM_95C_REG_00_FIFO, data, len);
    spiWrite(RFM_95C_REG_22_PAYLOAD_LENGTH, len + RFM_95C_HEADER_LEN);

    setModeTx(); // Start the transmitter
    // when Tx is done, interruptHandler will fire and radio mode will return to STANDBY
    return true;
}

bool RFM_95C::printRegisters()
{
#ifdef RH_HAVE_SERIAL
    uint8_t registers[] = {0x01, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11, 0x12, 0x13, 0x014, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27};

    uint8_t i;
    for (i = 0; i < sizeof(registers); i++)
    {
        Serial.print(registers[i], HEX);
        Serial.print(": ");
        Serial.println(spiRead(registers[i]), HEX);
    }
#endif
    return true;
}

uint8_t RFM_95C::maxMessageLength()
{
    return RFM_95C_MAX_MESSAGE_LEN;
}

bool RFM_95C::setFrequency(float centre)
{
    // Frf = FRF / FSTEP
    uint32_t frf = (centre * 1000000.0) / RFM_95C_FSTEP;
    spiWrite(RFM_95C_REG_06_FRF_MSB, (frf >> 16) & 0xff);
    spiWrite(RFM_95C_REG_07_FRF_MID, (frf >> 8) & 0xff);
    spiWrite(RFM_95C_REG_08_FRF_LSB, frf & 0xff);
    _usingHFport = (centre >= 779.0);

    return true;
}

void RFM_95C::setModeIdle()
{
    if (_mode != RHModeIdle)
    {
        spiWrite(RFM_95C_REG_01_OP_MODE, RFM_95C_MODE_STDBY);
        _mode = RHModeIdle;
    }
}

bool RFM_95C::sleep()
{
    if (_mode != RHModeSleep)
    {
        spiWrite(RFM_95C_REG_01_OP_MODE, RFM_95C_MODE_SLEEP);
        _mode = RHModeSleep;
    }
    return true;
}

void RFM_95C::setModeRx()
{
    if (_mode != RHModeRx)
    {
        spiWrite(RFM_95C_REG_01_OP_MODE, RFM_95C_MODE_RXCONTINUOUS);
        spiWrite(RFM_95C_REG_40_DIO_MAPPING1, 0x00); // Interrupt on RxDone
        _mode = RHModeRx;
    }
}

void RFM_95C::setModeTx()
{
    if (_mode != RHModeTx)
    {
        spiWrite(RFM_95C_REG_01_OP_MODE, RFM_95C_MODE_TX);
        spiWrite(RFM_95C_REG_40_DIO_MAPPING1, 0x40); // Interrupt on TxDone
        _mode = RHModeTx;
    }
}

void RFM_95C::setTxPower(int8_t power, bool useRFO)
{
    // Sigh, different behaviours depending on whther the module use PA_BOOST or the RFO pin
    // for the transmitter output
    if (useRFO)
    {
        if (power > 14)
            power = 14;
        if (power < -1)
            power = -1;
        spiWrite(RFM_95C_REG_09_PA_CONFIG, RFM_95C_MAX_POWER | (power + 1));
    }
    else
    {
        if (power > 23)
            power = 23;
        if (power < 5)
            power = 5;

        // For RFM_95C_PA_DAC_ENABLE, manual says '+20dBm on PA_BOOST when OutputPower=0xf'
        // RFM_95C_PA_DAC_ENABLE actually adds about 3dBm to all power levels. We will us it
        // for 21, 22 and 23dBm
        if (power > 20)
        {
            spiWrite(RFM_95C_REG_4D_PA_DAC, RFM_95C_PA_DAC_ENABLE);
            power -= 3;
        }
        else
        {
            spiWrite(RFM_95C_REG_4D_PA_DAC, RFM_95C_PA_DAC_DISABLE);
        }

        // RFM95/96/97/98 does not have RFO pins connected to anything. Only PA_BOOST
        // pin is connected, so must use PA_BOOST
        // Pout = 2 + OutputPower.
        // The documentation is pretty confusing on this topic: PaSelect says the max power is 20dBm,
        // but OutputPower claims it would be 17dBm.
        // My measurements show 20dBm is correct
        spiWrite(RFM_95C_REG_09_PA_CONFIG, RFM_95C_PA_SELECT | (power - 5));
    }
}

// Sets registers from a canned modem configuration structure
void RFM_95C::setModemRegisters(const ModemConfig *config)
{
    spiWrite(RFM_95C_REG_1D_MODEM_CONFIG1, config->reg_1d);
    spiWrite(RFM_95C_REG_1E_MODEM_CONFIG2, config->reg_1e);
    spiWrite(RFM_95C_REG_26_MODEM_CONFIG3, config->reg_26);
}

// Set one of the canned FSK Modem configs
// Returns true if its a valid choice
bool RFM_95C::setModemConfig(ModemConfigChoice index)
{
    if (index > (signed int)(sizeof(MODEM_CONFIG_TABLE) / sizeof(ModemConfig)))
        return false;

    ModemConfig cfg;
    memcpy_P(&cfg, &MODEM_CONFIG_TABLE[index], sizeof(RFM_95C::ModemConfig));
    setModemRegisters(&cfg);

    return true;
}

void RFM_95C::setPreambleLength(uint16_t bytes)
{
    spiWrite(RFM_95C_REG_20_PREAMBLE_MSB, bytes >> 8);
    spiWrite(RFM_95C_REG_21_PREAMBLE_LSB, bytes & 0xff);
}

bool RFM_95C::isChannelActive()
{
    // Set mode RHModeCad
    if (_mode != RHModeCad)
    {
        spiWrite(RFM_95C_REG_01_OP_MODE, RFM_95C_MODE_CAD);
        spiWrite(RFM_95C_REG_40_DIO_MAPPING1, 0x80); // Interrupt on CadDone
        _mode = RHModeCad;
    }

    while (_mode == RHModeCad)
        YIELD;

    return _cad;
}

void RFM_95C::enableTCXO()
{
    while ((spiRead(RFM_95C_REG_4B_TCXO) & RFM_95C_TCXO_TCXO_INPUT_ON) != RFM_95C_TCXO_TCXO_INPUT_ON)
    {
        sleep();
        spiWrite(RFM_95C_REG_4B_TCXO, (spiRead(RFM_95C_REG_4B_TCXO) | RFM_95C_TCXO_TCXO_INPUT_ON));
    }
}

// From section 4.1.5 of SX1276/77/78/79
// Ferror = FreqError * 2**24 * BW / Fxtal / 500
int RFM_95C::frequencyError()
{
    int32_t freqerror = 0;

    // Convert 2.5 bytes (5 nibbles, 20 bits) to 32 bit signed int
    // Caution: some C compilers make errors with eg:
    // freqerror = spiRead(RFM_95C_REG_28_FEI_MSB) << 16
    // so we go more carefully.
    freqerror = spiRead(RFM_95C_REG_28_FEI_MSB);
    freqerror <<= 8;
    freqerror |= spiRead(RFM_95C_REG_29_FEI_MID);
    freqerror <<= 8;
    freqerror |= spiRead(RFM_95C_REG_2A_FEI_LSB);
    // Sign extension into top 3 nibbles
    if (freqerror & 0x80000)
        freqerror |= 0xfff00000;

    int error = 0; // In hertz
    float bw_tab[] = {7.8, 10.4, 15.6, 20.8, 31.25, 41.7, 62.5, 125, 250, 500};
    uint8_t bwindex = spiRead(RFM_95C_REG_1D_MODEM_CONFIG1) >> 4;
    if (bwindex < (sizeof(bw_tab) / sizeof(float)))
        error = (float)freqerror * bw_tab[bwindex] * ((float)(1L << 24) / (float)RFM_95C_FXOSC / 500.0);
    // else not defined

    return error;
}

int RFM_95C::lastSNR()
{
    return _lastSNR;
}
