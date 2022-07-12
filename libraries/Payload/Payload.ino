#include "TShirtCannonPayload.h"

TShirtCannonPayload cannon;
void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // cannon.print();
    uint8_t transmission[7];
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

    cannon.print();
    delay(2000);
}
