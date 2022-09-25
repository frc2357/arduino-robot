#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include <RotaryEncoder.h>

class RotaryKnobController
{
public:
    RotaryKnobController(unsigned int pinClk, unsigned int pinDt);
    ~RotaryKnobController();
    int getValue();
    void nextMode();
    int getMode();

    static const int FIRING_TIME_ADJUST_MODE = 0;
    static const int ANGLE_ADJUST_MODE = 1;
    static const int MIN_MODE_VALUE = FIRING_TIME_ADJUST_MODE;
    static const int MAX_MODE_VALUE = ANGLE_ADJUST_MODE;

private:
    int m_pos = 0;
    int m_mode = 0;
    RotaryEncoder *m_encoder;
};
#endif