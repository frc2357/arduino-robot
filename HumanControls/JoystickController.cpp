#include "JoystickController.h"

JoystickController::JoystickController(unsigned int xPin,
                                       unsigned int yPin,
                                       unsigned int xDeadZoneSize,
                                       unsigned int yDeadZoneSize,
                                       unsigned int xMin,
                                       unsigned int xMax,
                                       unsigned int yMin,
                                       unsigned int yMax)
{
    this->xPin = xPin;
    this->yPin = yPin;

    this->xMin = xMin;
    this->xMax = xMax;

    this->yMin = yMin;
    this->yMax = yMax;

    this->xCenter = static_cast<double>(this->xMax) / 2;

    this->yCenter = static_cast<double>(this->yMax) / 2;

    this->xDeadMin = this->xCenter - (static_cast<double>(xDeadZoneSize) / 2);
    this->xDeadMax = this->xCenter + (static_cast<double>(xDeadZoneSize) / 2);

    this->yDeadMin = this->yCenter - (static_cast<double>(yDeadZoneSize) / 2);
    this->yDeadMax = this->yCenter + (static_cast<double>(yDeadZoneSize) / 2);

    this->xHalfTotal = this->xCenter - (this->xDeadMax - this->xCenter);
    this->xOffset = this->xCenter + (this->xDeadMax - this->xCenter);

    this->yHalfTotal = this->yCenter - (this->yDeadMax - this->yCenter);
    this->yOffset = this->yCenter + (this->yDeadMax - this->yCenter);

    this->x = 0;
    this->y = 0;
}
double JoystickController::getX()
{
    return this->x;
}
double JoystickController::getY()
{
    return this->y;
}

void JoystickController::update()
{
    this->x = calcX(analogRead(xPin));
    this->y = calcY(analogRead(yPin));
}

double JoystickController::calcX(int rawValue)
{
    double value = static_cast<double>(rawValue);
    if (value >= this->xDeadMin && value <= this->xDeadMax)
    {
        return 0.0;
    }

    if (value > this->xCenter)
    {
        return (value - this->xOffset) / this->xHalfTotal;
    }
    if (value < this->xCenter)
    {
        return -1 * (1 - (value / this->xHalfTotal));
    }

    return 0.0;
}

double JoystickController::calcY(int rawValue)
{
    double value = static_cast<double>(rawValue);
    if (value >= this->yDeadMin && value <= this->yDeadMax)
    {
        return 0.0;
    }

    if (value > this->yCenter)
    {
        return (value - this->yOffset) / this->yHalfTotal;
    }
    if (value < this->yCenter)
    {
        return -1 * (1 - (value / this->yHalfTotal));
    }

    return 0.0;
}