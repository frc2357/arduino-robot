#include "RobotComms.h"

const uint8_t RobotComms::PREAMBLE_LEN = 4;

RobotComms::RobotComms(unsigned int radioSS, unsigned int radioINT, unsigned int i2cHostAddress, unsigned int i2cDeviceAddress)
 : m_radio(radioSS, radioINT), m_commsI2C(i2cHostAddress, i2cDeviceAddress, PREAMBLE_LEN) {

}

void RobotComms::init(unsigned int radioFreq) {
    if (!m_radio.init())
        Serial.println("radio init failed");

    if (!m_radio.setFrequency(radioFreq))
    {
        Serial.println("radio setFrequency failed");
        while (1)
            ;
    }

    m_radio.setTxPower(23, false);

    m_commsI2C.init();

    Serial.println("Finished init");
}

void RobotComms::update() {
    uint8_t payloadLen = PAYLOAD_LEN;

    if (m_radio.recv(m_payload, &payloadLen)) {
        Serial.println((uint8_t)m_payload);

        // Send down serial if full message received
        if(payloadLen == PAYLOAD_LEN) {
            m_commsI2C.sendBytes(m_payload, PAYLOAD_LEN);
        }
    }
}