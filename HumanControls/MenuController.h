#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "Arduino.h"
#include "Page.h"
#include "DashPage.h"
#include "ElevatorPage.h"
#include "ShotPage.h"
#include "ValvePage.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class MenuController
{
public:
    MenuController(unsigned int encoderPinClk,
                   unsigned int encoderPinDt,
                   unsigned int displayAddress,
                   unsigned int displayLen,
                   unsigned int displayWidth,
                   unsigned int menuSize,
                   unsigned int angleIncrement,
                   unsigned int angleMin,
                   unsigned int angleMax,
                   unsigned int pressureIncrement,
                   unsigned int pressureMin,
                   unsigned int pressureMax,
                   unsigned int durationIncrement,
                   unsigned int durationMin,
                   unsigned int durationMax);
    void init();
    void menuUpdate();
    void menuPress();

private:
    RotaryKnobController rotaryKnob;
    DisplayController display;
    int rotation;
    bool isActive;
    Page *currentPage;
    DashPage dashPage;
    ElevatorPage elevatorPage;
    ShotPage shotPage;
    ValvePage valvePage;
};
#endif