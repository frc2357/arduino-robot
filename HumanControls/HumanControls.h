#ifndef HUMAN_CONTROLS
#define HUMAN_CONTROLS

#include "MenuController.h"
#include "FTDebouncer.h"
#include "EnableController.h"
#include "FireController.h"

class HumanControls
{
public:
    HumanControls(unsigned int encoderPinClk,
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
                  unsigned int numButtons,
                  unsigned int encoderPinSW,
                  unsigned int enablePin,
                  unsigned int firePin);
    void init();
    void update();
    void setStatus();
    void onPinActivated(int pinNr);
    void onPinDeactivated(int pinNr);
    void connect();

    static const char *disabled;
    static const char *enabled;
    static const char *primed;
    static const char *status;

private:
    int encoderPinSW, enablePin, firePin;
    bool isConnected;
    static const char *lastStatus;

    MenuController menuController;
    FTDebouncer pinDebouncer;
    EnableController enableController;
    FireController fireController;
};

#endif