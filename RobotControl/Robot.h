#ifndef ROBOTSTATE_H
#define ROBOTSTATE_H

#include "Arduino.h"
#include "RobotConstants.h"
#include "StatusLEDs.h"

class Robot {
  public:
    Robot(RobotPins pins);

    RobotStatus GetStatus();

    void Setup();

    void Update(int loopCount);

    void Disable();

    void Enable();

  private:
    RobotPins pins;
    RobotStatus status;

    StatusLEDs statusLEDs;
};

#endif // ROBOTSTATE_H
