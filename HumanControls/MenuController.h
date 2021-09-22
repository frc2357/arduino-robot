#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include "Arduino.h"
#include <JsonEl.h>
#include "Page.h"
#include "DashPage.h"
#include "ElevatorPage.h"
#include "ShotPage.h"
#include "ValvePage.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"
#include "FireController.h"

class MenuController
{
public:
    MenuController(unsigned int encoderPinClk,
                   unsigned int encoderPinDt,
                   unsigned int displayAddress,
                   unsigned int displayLen,
                   unsigned int displayWidth,
                   unsigned int angleIncrement,
                   unsigned int angleMin,
                   unsigned int angleMax,
                   unsigned int pressureIncrement,
                   unsigned int pressureMin,
                   unsigned int pressureMax,
                   unsigned int durationIncrement,
                   unsigned int durationMin,
                   unsigned int durationMax);
    void init(JsonState &state);
    void menuUpdate(JsonState &state);
    void menuRefresh(JsonState &state);
    void menuPress(JsonState &state, bool isEnabled, FireController &FireController);

private:
    RotaryKnobController m_rotaryKnob;
    DisplayController m_display;
    int m_rotation;
    bool m_isActive;
    Page *m_currentPage;
    DashPage m_dashPage;
    ElevatorPage m_elevatorPage;
    ShotPage m_shotPage;
    ValvePage m_valvePage;
};
#endif