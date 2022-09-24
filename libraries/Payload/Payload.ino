#include "TShirtCannonPayload.h"

TShirtCannonPayload cannon;
void setup()
{
    Serial.begin(115200);
    cannon.setStatus(1);
    cannon.setControllerDriveRight(128);
    cannon.setFiringPressure(1);
}

void loop()
{
    uint8_t transmission[7] = {};
    cannon.print();
    if (cannon.buildTransmission(transmission, sizeof(transmission)))
    {
        Serial.println("Transmit build successfull");
    }
    else
    {
        Serial.println("Transmit fail");
    }

    if (cannon.readMessage(transmission, sizeof(transmission)))
    {
        Serial.println("Read Successfull");
    }
    else
    {
        Serial.println("Read fail");
    }
    delay(2000);
}
