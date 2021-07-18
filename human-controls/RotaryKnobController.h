#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include "MenuInterface.h"
#include <RotaryEncoder.h>

class RotaryKnobController : public MenuInterface
{
public:
    RotaryKnobController(unsigned int pinClk, unsigned int pinDt);
    ~RotaryKnobController();
    void updateRotaryKnob();
    void menuUp();
    void menuDown();
    void menuSelect();

private:
    int pinSw, pos = 0, lastClick = -1;
    RotaryEncoder *encoder;
};
#endif