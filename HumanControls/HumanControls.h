#ifndef HUMAN_CONTROLS_H
#define HUMAN_CONTROLS_H

#include <Arduino.h>
#include <JsonElement.h>
#include "MenuController.h"
#include "FTDebouncer.h"
#include "EnableController.h"
#include "FireController.h"
#include "JoystickAxis.h"
#include "RFM95C.h"
#include "Utils.h"
#include "LinkedList.h"
#include "CommunicationDriver.h"

class HumanControls
{
public:
    HumanControls(TShirtCannonPayload &payload, LinkedList &messageQueue,
                  unsigned int encoderPinA,
                  unsigned int encoderPinB,
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
                  unsigned int yDeadZoneSize,
                  unsigned int rfm95_cs,
                  unsigned int rfm95_int,
                  unsigned int rfm95_freq,
                  unsigned int rfm95_txPower);
    void init();
    void update();
    void setStatus();
    void onPinActivated(int pinNr);
    void onPinDeactivated(int pinNr);
    void connect();

    static Utils::ControllerStatus status;

private:
    void setError(const char *format, ...);

    int m_encoderPinSW, m_enablePin, m_primePin, m_firePin;
    bool m_isConnected;
    static Utils::ControllerStatus lastStatus;

    // RFM_95C m_rawDriver;
    CommunicationDriver m_commDriver;
    int m_rfm95Freq, m_rfm95TxPower;

    TShirtCannonPayload &m_payload;

    MenuController m_menuController;
    FTDebouncer m_pinDebouncer;
    EnableController m_enableController;
    FireController m_fireController;
    JoystickAxis m_leftStick, m_rightStick;
    LinkedList m_messageQueue;
};

#endif
