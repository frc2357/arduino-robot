#ifndef JOYSTICKCONTROLLER_H
#define JOYSTICKCONTROLLER_H

#include "Arduino.h"
#include "CtrlUtil.h"

class JoystickController
{
public:
    JoystickController(int xPin, int yPin, double deadZoneSize); //deadZoneSize is the total proportion of the joystick's range of values to omit
    double getX();
    double getY();

private:
    double calcReturnValue(double value);

    int xPin, yPin;
    double min, max;
};

#endif