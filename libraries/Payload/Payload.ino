#include "TShirtCannonPayload.h"

TShirtCannonPayload cannon;
void setup()
{
    Serial.begin(115200);
    // cannon.m_payload->message = 0B1111111;

//    Serial.print("Set as: ");
//    Serial.println(cannon.m_payload->message);
//    Serial.println("Converting to character array");
    uint8_t transmission = cannon.buildTransmission();
    
    // for (int i = 0; i < sizeof(transmission); i++) {
    //     Serial.println(transmission[i]);
    // }

//    cannon.readMessage(trans);
//    Serial.print("After conversions: ");
//    Serial.println(cannon.m_payload->message);

    // cannon.print();
}

void loop()
{
    // cannon.print();
}
