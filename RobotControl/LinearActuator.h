#ifndef LINEAR_ACTUATOR_H
#define LINEAR_ACTUATOR_H

#include <Arduino.h>
#include <Servo.h>

class LinearActuator
{
    static const int RETRACT;
    static const int STOP;
    static const int EXTEND;

    static const int RETRACT_WRITE_VALUE;
    static const int STOP_WRITE_VALUE;
    static const int EXTEND_WRITE_VALUE;

    static const int MAX_POS_MILLIS;
    static const int MIN_POS_MILLIS;

public:
    LinearActuator(int input);
    void init();
    void update(int direction);
    void extend();
    void retract();
    void stop();
    bool isMoving(int direction);

private:
    int m_inputPin, m_currentDirection, m_currentPos;
    unsigned long m_lastMillis;
    Servo m_actuator;
};

#endif