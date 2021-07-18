#include <RotaryEncoder.h>

#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include "MenuInterface.h"

class RotaryKnobController : public MenuInterface
{
public:
    RotaryKnobController(unsigned int, unsigned int);
    void updateRotaryKnob();
    void menuUp();
    void menuDown();
    void menuSelect();

private:
    int pinSw, pos = 0, lastClick = -1;
    RotaryEncoder *encoder;
};
#endif