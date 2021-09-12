#include "JoystickAxis.h"

JoystickAxis::JoystickAxis(unsigned int pin,
                           unsigned int deadZoneSize,
                           unsigned int min,
                           unsigned int max)
{
    this->pin = pin;

    this->result = 0;

    this->deadZoneSize = static_cast<double>(deadZoneSize);
    this->range = static_cast<double>(max - min);
    this->min = static_cast<double>(min);
}
double JoystickAxis::getResult()
{
    return this->result;
}

void JoystickAxis::update()
{
    this->result = calcResult(analogRead(pin));
}

double JoystickAxis::calcResult(int rawValue)
{
    double value = rawValue;

    if (value >= min && value >= (range + min))
    {
        return 0.0;
    }

    if (value >= ((this->range / 2) - (this->deadZoneSize / 2)) && value <= ((this->range / 2) + (this->deadZoneSize / 2)))
    {
        return 0.0;
    }

    if (value > (this->range / 2))
    {
        return (value - ((this->range / 2) + (((this->range / 2) + (this->deadZoneSize / 2)) - (this->range / 2)))) / ((this->range / 2) - (((this->range / 2) + (this->deadZoneSize / 2)) - (this->range / 2)));
    }
    if (value < (this->range / 2))
    {
        return -1 * (1 - (value / ((this->range / 2) - (((this->range / 2) + (this->deadZoneSize / 2)) - (this->range / 2)))));
    }

    return 0.0;
}