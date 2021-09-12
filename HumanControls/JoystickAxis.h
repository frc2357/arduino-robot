#ifndef JOYSTICK_AXIS_H
#define JOYSTICK_AXIS_H

#include "Arduino.h"

class JoystickAxis
{
public:
    //deadZoneSize is the total proportion of the joystick's range of values to omit
    JoystickAxis(unsigned int pin,
                 unsigned int deadZoneSize,
                 unsigned int min,
                 unsigned int max);
    void update();
    double getResult();

private:
    double calcResult(int rawValue);

    int pin;
    double result, deadMin, deadMax, center,
        halfTotal, offset;
};
#endif