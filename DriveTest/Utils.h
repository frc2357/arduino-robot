#ifndef UTILS_H
#define UTILS_H

#include "TShirtCannonPayload.h"
#include "RotaryEncoder.h"

class Utils
{
public:
    static void setMotors(TShirtCannonPayload &payload, float x, float y);
    static void setAngle(TShirtCannonPayload &payload, RotaryEncoder &encoder);
};

#endif
