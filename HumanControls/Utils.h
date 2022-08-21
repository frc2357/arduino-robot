#ifndef UTILS_H
#define UTILS_H

#include "TShirtCannonPayload.h"

class Utils
{
public:
    static void setMotors(TShirtCannonPayload payload, float x, float y);

private:
    const uint8_t MAX_DRIVE_SPEED = 63;
};

#endif