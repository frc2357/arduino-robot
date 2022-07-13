#include "HumanControls.h"

const char *HumanControls::STATUS_DISABLED = "Disabled";
const char *HumanControls::STATUS_ENABLED = "Enabled";
const char *HumanControls::STATUS_PRIMED = "Primed";
const char *HumanControls::status = HumanControls::STATUS_DISABLED;
const char *HumanControls::lastStatus = HumanControls::STATUS_DISABLED;

HumanControls::HumanControls(TShirtCannonPayload &payload, unsigned int encoderPinClk,
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
                             unsigned int enablePin,
                             unsigned int primePin,
                             unsigned int firePin,
                             unsigned int joystickPinVRX,
                             unsigned int xDeadZoneSize,
                             unsigned int joystickMax,
                             unsigned int joystickPinVRY,
                             unsigned int yDeadZoneSize)
    : m_payload(payload),
      m_menuController(encoderPinClk, encoderPinDt, displayAddress, displayLen, displayWidth,
                       angleIncrement, angleMin, angleMax, pressureIncrement, pressureMin,
                       pressureMax, durationIncrement, durationMin, durationMax, hangTimerDuration,
                       downArrow, upArrow, robotBatChar, controllerBatChar),
      m_pinDebouncer(numButtons), m_enableController(), m_fireController(),
      m_leftStick(joystickPinVRX, xDeadZoneSize, joystickMax),
      m_rightStick(joystickPinVRY, yDeadZoneSize, joystickMax)
{
    this->m_encoderPinSW = encoderPinSW;
    this->m_enablePin = enablePin;
    this->m_primePin = primePin;
    this->m_firePin = firePin;
    this->m_isConnected = false;
}

void HumanControls::init(unsigned int downArrow, unsigned int upArrow)
{
    connect();
    m_pinDebouncer.addPin(this->m_encoderPinSW, HIGH, INPUT_PULLUP);
    m_pinDebouncer.addPin(this->m_enablePin, HIGH, INPUT_PULLUP);
    m_pinDebouncer.addPin(this->m_primePin, HIGH, INPUT_PULLUP);
    m_pinDebouncer.addPin(this->m_firePin, HIGH, INPUT_PULLUP);
    m_pinDebouncer.begin();
    m_menuController.init(m_payload, downArrow, upArrow);
}

void HumanControls::update()
{
    if (m_isConnected == false)
    {
        this->connect();
    }

    this->setStatus();

    m_menuController.menuUpdate(m_payload, status == HumanControls::STATUS_ENABLED);
    m_pinDebouncer.update();

    m_rightStick.update();
    m_leftStick.update();
    // TODO: Convert joystick values to motor speeds for m_payload
    // m_payload.root()["x"] = m_leftStick.getResult();
    // m_payload.root()["y"] = m_rightStick.getResult();
    Serial.println("vel: " + String(m_rightStick.getResult()));
    // Serial.println("turn: " + String(m_leftStick.getResult()));
}

void HumanControls::setStatus()
{

    if (this->m_isConnected)
    {
        if (strlen(m_payload.m_error.asString()) == 0)
        {
            // Check the robot's status and see if it is different then the controllers.
            // If it is, set the status to the lowest given status. Ex. Robot returns disabled, but controller enabled, so
            // set both statuses to disabled. Robot status and controller status should always be the same.
            // Only exception may be due to firing setting status to enabled after firing.

            if (m_enableController.getIsEnabled())
            {
                if (m_fireController.getIsPrimed())
                {
                    status = HumanControls::STATUS_PRIMED;
                }
                else
                {
                    status = HumanControls::STATUS_ENABLED;
                }
            }
            else
            {
                status = HumanControls::STATUS_DISABLED;
            }
        }
        else
        {
            status = HumanControls::STATUS_DISABLED;
        }
    }
    else
    {
        status = HumanControls::STATUS_DISABLED;
        // m_payload.root()["hCtl"]["conn"] = false; Potentially not used
    }

    if (status != lastStatus)
    {
        Serial.println(status);
        lastStatus = status;
        m_payload.m_status = status;
        m_menuController.menuRefresh(m_payload);
    }

    // Enventually will set the robot's status here
}

// Methods for debouncer
void HumanControls::onPinActivated(int pinNr)
{
    if (pinNr == m_encoderPinSW)
    {
        m_menuController.menuPress(m_payload, (status == HumanControls::STATUS_ENABLED));
    }
    else if (pinNr == m_enablePin)
    {
        m_enableController.setIsEnabled(true);
    }
    else if (pinNr == m_primePin)
    {
        if (status == HumanControls::STATUS_ENABLED)
        {
            m_fireController.setIsPrimed(true);
        }
        else
        {
            m_fireController.setIsPrimed(false);
        }
    }
    else if (pinNr == m_firePin)
    {
        if (HumanControls::status == HumanControls::STATUS_PRIMED && m_isConnected)
        {
            m_fireController.initiateFiring(true);
            m_fireController.setIsPrimed(false);
        }
    }
}

void HumanControls::onPinDeactivated(int pinNr)
{
    if (pinNr == m_enablePin)
    {
        m_enableController.setIsEnabled(false);
        m_fireController.setIsPrimed(false);
    }
}

void HumanControls::connect()
{
    // Connect to the robot
    this->m_isConnected = true;
    // m_payload.root()["hCtl"]["conn"] = true; Potentially not used
}

void HumanControls::setError(const char *format, ...)
{
    va_list args;
    char message[32];
    va_start(args, format);
    vsprintf(message, format, args);
    va_end(args);

    status = HumanControls::STATUS_DISABLED;
    m_payload.m_status = HumanControls::STATUS_DISABLED;

    Serial.print("ERROR: ");
    Serial.println(message);
}
