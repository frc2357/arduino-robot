#include "JoystickAxis.h"

JoystickAxis::JoystickAxis(unsigned int pin,
                           unsigned int deadZoneSize,
                           unsigned int max)
{
    this->m_pin = pin;
    this->m_deadZoneSize = deadZoneSize;
    this->m_max = max;

    this->m_result = 0;
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
    //General calculations
    //Assumes that this is the raw value the joystick returns when it is untouched
    int center = m_max / 2;
    int halfDeadZoneSize = m_deadZoneSize / 2;

    int deadMin = center - halfDeadZoneSize;
    int deadMax = center + halfDeadZoneSize;

    //Finalizing result
    if (rawValue > deadMax)
    {
        return calcRange(deadMax, m_max, rawValue);
    }
    if (rawValue < deadMin)
    {
        return -1 * (1 - calcRange(0, deadMin, rawValue));
    }

    return 0.0;
}

double JoystickAxis::calcRange(int min, int max, int current)
{
    int halfTotal = max - min;

    return static_cast<double>((current - min)) / halfTotal;
}