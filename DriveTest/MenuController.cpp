#include "MenuController.h"
#include "TestPage.h"

MenuController::MenuController(unsigned int encoderPinClk,
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
                               unsigned int hangTimerDuration)
  : m_rotaryKnob(encoderPinClk, encoderPinDt), m_display(), m_testPage()
{
  this->m_isActive = false;

  this->m_rotation = 0;

  
}