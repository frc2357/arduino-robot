#include "TShirtCannonPayload.h"

TShirtCannonPayload cannon;
void setup()
{
    Serial.begin(115200);
}

void loop()
{
    // cannon.print();
    uint8_t *transmission = cannon.buildTransmission();
    delay(2000);
}
