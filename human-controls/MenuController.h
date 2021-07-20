#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "DisplayPage.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class MenuController : DisplayPage
{
public:
    MenuController(unsigned int encoderPinClk, unsigned int encoderPinDt, unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth);
    ~MenuController();
    void menuInit();
    void clockwisedate();
    void menuPress();
    void clockwise();
    void counterClockwise();
    void menuSelect();

private:
    RotaryKnobController *rotaryKnob;
    DisplayController *display;
};
#endif