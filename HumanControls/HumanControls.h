#ifndef HUMAN_CONTROLS
#define HUMAN_CONTROLS

#include <Arduino.h>
#include <JsonEl.h>
#include "MenuController.h"
#include "FTDebouncer.h"
#include "EnableController.h"
#include "FireController.h"
#include "JoystickAxis.h"

class HumanControls
{
public:
    HumanControls(JsonState &state,
                  unsigned int encoderPinClk,
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
                  unsigned int durationMax,
                  unsigned int hangTimerDuration,
                  unsigned int downArrow,
                  unsigned int upArrow,
                  unsigned int robotBatChar,
                  unsigned int controllerBatChar,
                  unsigned int numButtons,
                  unsigned int encoderPinSW,
                  unsigned int primePin,
                  unsigned int enablePin,
                  unsigned int firePin,
                  unsigned int joystickPinVRX,
                  unsigned int xDeadZoneSize,
                  unsigned int joystickMax,
                  unsigned int joystickPinVRY,
                  unsigned int yDeadZoneSize);
    void init(unsigned int downArrow, unsigned int upArrow);
    void update();
    void setStatus();
    void onPinActivated(int pinNr);
    void onPinDeactivated(int pinNr);
    void connect();

    static const char *STATUS_DISABLED;
    static const char *STATUS_ENABLED;
    static const char *STATUS_PRIMED;
    static const char *status;

private:
    void setError(const char *format, ...);

    int m_encoderPinSW, m_enablePin, m_primePin, m_firePin;
    bool m_isConnected;
    static const char *lastStatus;

    JsonState &m_state;

    MenuController m_menuController;
    FTDebouncer m_pinDebouncer;
    EnableController m_enableController;
    FireController m_fireController;
    JoystickAxis m_leftStick, m_rightStick;
};

#endif