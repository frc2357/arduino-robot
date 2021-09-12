#include "HumanControls.h"

const char *HumanControls::disabled = "disabled";
const char *HumanControls::enabled = "enabled";
const char *HumanControls::primed = "primed";
const char *HumanControls::status = HumanControls::disabled;
const char *HumanControls::lastStatus = HumanControls::disabled;

HumanControls::HumanControls(unsigned int encoderPinClk,
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
                             unsigned int firePin)
    : menuController(encoderPinClk, encoderPinDt, displayAddress, displayLen, displayWidth,
                     angleIncrement, angleMin, angleMax, pressureIncrement, pressureMin,
                     pressureMax, durationIncrement, durationMin, durationMax),
      pinDebouncer(numButtons), enableController(), fireController() //,
                                                                     /*encoderPinSW(encoderPinSW), enablePin(enablePin), firePin(firePin)*/
{
    this->encoderPinSW = encoderPinSW;
    this->enablePin = enablePin;
    this->firePin = firePin;
    this->isConnected = false;
}

void HumanControls::init()
{
    connect();
    pinDebouncer.addPin(this->encoderPinSW, HIGH, INPUT_PULLUP);
    pinDebouncer.addPin(this->enablePin, HIGH, INPUT_PULLUP);
    pinDebouncer.addPin(this->firePin, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();
    menuController.init(status);
}

void HumanControls::update()
{
    if (isConnected == false)
    {
        this->connect();
    }

    this->setStatus();

    menuController.menuUpdate(status);
    pinDebouncer.update();
}

void HumanControls::setStatus()
{

    if (this->isConnected)
    {
        //Check the robot's status and see if it is different then the controllers.
        //If it is, set the status to the lowest given status. Ex. Robot returns disabled, but controller enabled, so
        //set both statuses to disabled. Robot status and controller status should always be the same.
        //Only exception may be due to firing setting status to enabled after firing.

        if (enableController.getIsEnabled())
        {
            if (fireController.getIsFireToggled())
            {
                status = HumanControls::primed;
            }
            else
            {
                status = HumanControls::enabled;
            }
        }
        else
        {
            status = HumanControls::disabled;
        }
    }
    else
    {
        status = HumanControls::disabled;
    }

    if (status != lastStatus)
    {
        Serial.println(status);
        lastStatus = status;
        menuController.menuRefresh(status);
    }

    //Enventually will set the robot's status here
}

//Methods for debouncer
void HumanControls::onPinActivated(int pinNr)
{
    if (pinNr == encoderPinSW)
    {
        menuController.menuPress(status, (status == HumanControls::enabled), fireController);
    }
    else if (pinNr == enablePin)
    {
        enableController.setIsEnabled(true);
    }
    else if (pinNr == firePin)
    {
        if (HumanControls::status == HumanControls::primed && isConnected)
        {
            fireController.initiateFiring(true);
            fireController.setIsFireToggled(false);
        }
    }
}

void HumanControls::onPinDeactivated(int pinNr)
{
    if (pinNr == enablePin)
    {
        enableController.setIsEnabled(false);
        fireController.setIsFireToggled(false);
    }
}

void HumanControls::connect()
{
    //Connect to the robot
    this->isConnected = true;
}
