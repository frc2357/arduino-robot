#ifndef UTILS_H
#define UTILS_H

#include "TShirtCannonPayload.h"

class Utils
{
public:
  enum ControllerStatus
  {
    DISCONNECTED = 0,
    DISABLED = 1,
    ENABLED = 2,
    PRIMED = 3
  };
  static void setMotors(TShirtCannonPayload &payload, float x, float y);
};

#endif
