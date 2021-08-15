#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

#include "Arduino.h"
#include "RobotConstants.h"
#include "StatusLEDs.h"
#include "JsonState.h"

class Robot {
  public:
    Robot(RobotPins pins);

    RobotStatus GetStatus();

    void Init();

    void Update();

    void Disable();

    void Enable();

  private:
    RobotPins pins;
    RobotStatus status;

    JsonState jsonState;
    StatusLEDs statusLEDs;
};

#endif // ROBOTSTATE_H
