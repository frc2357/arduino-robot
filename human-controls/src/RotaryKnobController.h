#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include <RotaryEncoder.h>

class RotaryKnobController
{
public:
    RotaryKnobController(unsigned int pinClk, unsigned int pinDt);
    ~RotaryKnobController();
    int getValue();

private:
    int pos = 0;
    RotaryEncoder *encoder;
};
#endif