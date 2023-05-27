#ifndef MENUCONTROLLER_H
#define MENUCONTROLLER_H

#include <Arduino.h>
#include "Page.h"
#include "TestPage.h"

class MenuController
{
public:
  MenuController(unsigned int encoderPinClk,
                 unsigned int encoderPinDt,
                 unsigned int angleIncrement,
                 unsigned int angleMin,
                 unsigned int angleMax,
                 unsigned int pressureIncrement,
                 unsigned int pressureMin,
                 unsigned int pressureMax,
                 unsigned int durationIncrement,
                 unsigned int durationMin,
                 unsigned int durationMax,
                 unsigned int hangTimerDuration);
  void init(TShirtCannonPayload &payload);
  void menuUpdate(TShirtCannonPayload &payload, bool isEnabled);
  void menuRefresh(TShirtCannonPayload &payload);
  void menuPress(TShirtCannonPayload &payload, bool isEnabled);
private:
  RotaryKnobController m_rotaryKnob;
  DisplayController m_display;
  int m_rotation, m_hangTimerDuration;
  long m_time;
  bool m_isActive;
  Page *m_currentPage;
  TestPage m_testPage;

};
#endif