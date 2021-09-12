#include "JoystickAxis.h"

JoystickAxis::JoystickAxis(unsigned int pin,
                           unsigned int deadZoneSize,
                           unsigned int min,
                           unsigned int max)
{
    this->m_pin = pin;

    this->m_result = 0;

    this->m_deadZoneSize = static_cast<double>(deadZoneSize);
    this->m_range = static_cast<double>(max - min);
    this->m_min = static_cast<double>(min);
}
double JoystickAxis::getResult()
{
    return this->m_result;
}

void JoystickAxis::update()
{
    this->m_result = calcResult(analogRead(m_pin));
}

double JoystickAxis::calcResult(int rawValue)
{
    double value = rawValue;

    if (value >= m_min && value >= (m_range + m_min))
    {
        return 0.0;
    }

    if (value >= ((this->m_range / 2) - (this->m_deadZoneSize / 2)) && value <= ((this->m_range / 2) + (this->m_deadZoneSize / 2)))
    {
        return 0.0;
    }

    if (value > (this->m_range / 2))
    {
        return (value - ((this->m_range / 2) + (((this->m_range / 2) + (this->m_deadZoneSize / 2)) - (this->m_range / 2)))) / ((this->m_range / 2) - (((this->m_range / 2) + (this->m_deadZoneSize / 2)) - (this->m_range / 2)));
    }
    if (value < (this->m_range / 2))
    {
        return -1 * (1 - (value / ((this->m_range / 2) - (((this->m_range / 2) + (this->m_deadZoneSize / 2)) - (this->m_range / 2)))));
    }

    return 0.0;
}