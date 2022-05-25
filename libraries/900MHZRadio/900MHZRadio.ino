// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RFM_95C class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <RHDatagram.h>
#include "RFM95C.h"
#include <SPI.h>

// for feather m0
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 915.0

#define CONTROLLER_ADDRESS 1
#define ROBOT_ADDRESS 2

// Singleton instance of the radio driver
RFM_95C raw_driver(RFM95_CS, RFM95_INT);

void setup()
{
    Serial.begin(115200);
    while (!Serial)
        ; // Wait for serial port to be available
    if (!raw_driver.init())
        Serial.println("init failed");
    // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on

    if (!raw_driver.setFrequency(RF95_FREQ))
    {
        Serial.println("setFrequency failed");
        while (1)
            ;
    }

    // The default transmitter power is 13dBm, using PA_BOOST.
    // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
    // you can set transmitter powers from 5 to 23 dBm:
    raw_driver.setTxPower(23, false);

    Serial.println("Finished init");
}

uint8_t data[] = {1, 2, 3, 4, 5, 6, 7};

void loop()
{
    uint8_t from;

    raw_driver.send(data, sizeof(data));
    int time = millis();

    raw_driver.waitPacketSent();

    Serial.println("Packet sent in" + String(millis() - time));

    // Serial.println("Controller received a message");
    //  Wait for a message addressed to us from the client

    // if (manager.recvfrom(buf, &len, &from))
    // {
    //     Serial.print("got request from : 0x");
    //     Serial.print(from, HEX);
    //     Serial.print(": ");
    //     Serial.println((char *)buf);
    //     // Send a reply back to the originator client
    // }
}
