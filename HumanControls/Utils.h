#ifndef UTILS_H
#define UTILS_H

#include "TShirtCannonPayload.h"

class Utils
{
public:
  enum ControllerStatus
  {
    DISABLED = 0,
    ENABLED = 1,
    PRIMED = 2
  };
  static void setMotors(TShirtCannonPayload &payload, float x, float y);
};

#endif
