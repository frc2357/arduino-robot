#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "Arduino.h"
#include "Page.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class MenuController
{
public:
    MenuController(unsigned int encoderPinClk, unsigned int encoderPinDt,
                   unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth,
                   unsigned int menuSize);
    void init(Page pages[]);
    void menuUpdate();
    void menuPress();

private:
    RotaryKnobController rotaryKnob;
    DisplayController display;
    int rotation, menuIndex, menuSize, lastMenuIndex;
    Page *pages;
    bool isActive;
};
#endif