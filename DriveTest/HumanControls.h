#ifndef HUMAN_CONTROLS
#define HUMAN_CONTROLS

#include <Arduino.h>
#include <JsonElement.h>
#include "MenuController.h"
#include "FTDebouncer.h"
#include "EnableController.h"
#include "FireController.h"
#include "JoystickAxis.h"

class HumanControls
{
public:
    HumanControls(TShirtCannonPayload &payload,
                  unsigned int encoderPinClk,
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
                  unsigned int hangTimerDuration,
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

    static const uint8_t STATUS_DISABLED;
    static const uint8_t STATUS_ENABLED;
    static const uint8_t STATUS_PRIMED;
    static uint8_t status;

private:
    void setError(const char *format, ...);

    int m_encoderPinSW, m_enablePin, m_primePin, m_firePin;
    bool m_isConnected;
    static uint8_t lastStatus;

    TShirtCannonPayload &m_payload;

    MenuController m_menuController;
    FTDebouncer m_pinDebouncer;
    EnableController m_enableController;
    FireController m_fireController;
    JoystickAxis m_leftStick, m_rightStick;
};

#endif
