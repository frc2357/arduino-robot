#ifndef JOYSTICKCONTROLLER_H
#define JOYSTICKCONTROLLER_H

#include "Arduino.h"

class JoystickController
{
public:
    JoystickController(unsigned int xPin, unsigned int yPin, unsigned int xDeadZoneSize, unsigned int yDeadZoneSize,
                       unsigned int xMin, unsigned int xMax, unsigned int yMin, unsigned int yMax); //deadZoneSize is the total proportion of the joystick's range of values to omit
    void update();
    double getX();
    double getY();

private:
    double calcX(int rawValue);
    double calcY(int rawValue);

    int xPin, yPin, xMin, xMax, yMin, yMax;
    double x, y, xDeadMin, xDeadMax, yDeadMin, yDeadMax, xCenter, yCenter, xHalfTotal, xOffset, yHalfTotal, yOffset;
};

#endif