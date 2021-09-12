#include "JoystickAxis.h"

JoystickAxis::JoystickAxis(unsigned int pin,
                           unsigned int deadZoneSize,
                           unsigned int min,
                           unsigned int max)
{
    this->pin = pin;

    this->center = static_cast<double>(max) / 2;

    this->deadMin = this->center - (static_cast<double>(deadZoneSize) / 2);
    this->deadMax = this->center + (static_cast<double>(deadZoneSize) / 2);

    this->halfTotal = this->center - (this->deadMax - this->center);
    this->offset = this->center + (this->deadMax - this->center);

    this->result = 0;
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
    double value = static_cast<double>(rawValue);
    if (value >= this->deadMin && value <= this->deadMax)
    {
        return 0.0;
    }

    if (value > this->center)
    {
        return (value - this->offset) / this->halfTotal;
    }
    if (value < this->center)
    {
        return -1 * (1 - (value / this->halfTotal));
    }

    return 0.0;
}