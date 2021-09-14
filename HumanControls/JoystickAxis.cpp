#include "JoystickAxis.h"

JoystickAxis::JoystickAxis(unsigned int pin,
                           unsigned int deadZoneSize,
                           unsigned int min,
                           unsigned int max)
{
    this->m_pin = pin;

    this->m_result = 0;

    this->m_range = static_cast<double>(max - min);
    this->m_min = static_cast<double>(min);

    m_center = m_range / 2;

    m_halfDeadZoneSize = static_cast<double>(deadZoneSize) / 2;
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

    //General calculations
    double deadMin = m_center - m_halfDeadZoneSize;
    double deadMax = m_center - m_halfDeadZoneSize;

    //Assumes that this is the raw value the joystick returns when it is untouched
    double halfCenter = m_center - (deadMax - m_center);

    //Offset to ensure a value past the center of the joystick is scalable to the halfCenter
    double offset = m_center + (deadMax - m_center);

    //Finalizing result
    if (value <= m_min || value >= (m_range + m_min))
    {
        return 0.0;
    }

    if (value >= deadMin && value <= deadMax)
    {
        return 0.0;
    }

    if (value > m_center)
    {
        return (value - offset) / halfCenter;
    }
    if (value < m_center)
    {
        return -1 * (1 - (value / halfCenter));
    }

    return 0.0;
}