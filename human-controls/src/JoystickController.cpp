#include "JoystickController.h"

JoystickController::JoystickController(int xPin, int yPin, double deadZoneSize)
{
    this->xPin = xPin;
    this->yPin = yPin;
    this->min = 0.5 - (deadZoneSize / 2);
    this->max = (deadZoneSize / 2) + 0.5;
    this->center = 1023 / 2;
    this->remainingProportion = center - ((max - 0.5) * 1023);
}
double JoystickController::getX()
{
    return calcReturnValue(analogRead(xPin));
}
double JoystickController::getY()
{
    return calcReturnValue(analogRead(yPin));
}

double JoystickController::calcReturnValue(double value)
{
    double proportion = value / 1023;

    //Serial.println("Value: " + String(value));
    //Serial.println("Proportion: " + String(proportion));

    if (proportion >= min && proportion <= max)
    {
        return 0.0;
    }

    if (proportion > 0.5)
    {
        return (value - (center + ((max - 0.5) * 1023))) / remainingProportion;
    }
    if (proportion < 0.5)
    {
        return -1 * (1 - (value / remainingProportion));
    }

    return 0.0;
}