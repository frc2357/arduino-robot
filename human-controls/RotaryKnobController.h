#ifndef ENCODERCONTROLLER_H
#define ENCODERCONTROLLER_H

#include "DisplayPage.h"
#include <RotaryEncoder.h>

class RotaryKnobController : public DisplayPage
{
public:
    RotaryKnobController(unsigned int pinClk, unsigned int pinDt);
    //RotaryKnobController(unsigned int pinClk, unsigned int pinDt, void (MenuController::*clockwisePtr)(), void (MenuController::*counterClockwisePtr)(), void (MenuController::*menuSelectPtr)());
    ~RotaryKnobController();
    void updateRotaryKnob();
    void clockwise();
    void counterClockwise();
    void menuSelect();

private:
    int pinSw, pos = 0, lastClick = -1;
    RotaryEncoder *encoder;
    //void (MenuController::*clockwisePtr)();
    //void (MenuController::*counterClockwisePtr)();
    //void (MenuController::*menuSelectPtr)();
};
#endif