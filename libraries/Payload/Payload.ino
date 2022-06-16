#include "TShirtCannonPayload.h"

TShirtCannonPayload cannon;
void setup()
{
    Serial.begin(115200);
    cannon.m_payload.angle = 0B1111111;

    Serial.print("Set as: ");
    Serial.println(cannon.m_payload.angle);
    Serial.println("Converting to character array");
    uint8_t *trans = cannon.buildTransmission();
    cannon.readMessage(trans);
    Serial.print("After conversions: ");
    Serial.println(cannon.m_payload.angle);

    cannon.print();
}

void loop()
{
    // cannon.print();
}
