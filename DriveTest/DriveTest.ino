// ! IF USING THE CONTROLLER, MAKE SURE LINES 33-34 ARE NOT COMMENTED OUT

#include "JoystickAxis.h"
#include "Utils.h"
#include <TShirtCannonPayload.h>
#include <RFM95C.h>
#include "FTDebouncer.h"

#define JOYSTICK_PIN_VRX 0
#define JOYSTICK_PIN_VRY 1
#define POWER_DOWN_PIN 19

#define DEAD_ZONE_SIZE 100
#define JOYSTICK_MAX 1023

#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 3

#define RF95_FREQ 915.0

#define CONTROLLER_ADDRESS 1
#define ROBOT_ADDRESS 2

#define ENABLE_PIN 10      // Digital Pin for the enable button

RFM_95C raw_driver(RFM95_CS, RFM95_INT);

FTDebouncer m_pinDebouncer(1);

TShirtCannonPayload payload;

JoystickAxis leftStick(JOYSTICK_PIN_VRY, DEAD_ZONE_SIZE, JOYSTICK_MAX);
JoystickAxis rightStick(JOYSTICK_PIN_VRX, DEAD_ZONE_SIZE, JOYSTICK_MAX);

uint8_t buf[7];

void setup()
{
    // ! UNCOMMENT THE LINES BELOW IF USING CONTROLLER
    pinMode(POWER_DOWN_PIN, OUTPUT);
    digitalWrite(POWER_DOWN_PIN, HIGH);

    Serial.begin(115200);

    if (!raw_driver.init())
    {
        Serial.println("Driver init failed");
    }

    if (!raw_driver.setFrequency(RF95_FREQ))
    {
        Serial.println("Could not set driver frequency");
        while (1)
            ;
    }

    raw_driver.setTxPower(23, false);

    m_pinDebouncer.addPin(ENABLE_PIN, HIGH, INPUT_PULLUP);
    m_pinDebouncer.begin();
}

void loop()
{
    rightStick.update();
    leftStick.update();
    m_pinDebouncer.update();

    float turn, speed;
    turn = rightStick.getResult();
    speed = leftStick.getResult();

    if (payload.getMessageIndex() + 1 > 31) {
        payload.setMessageIndex(0);
    } else {
        payload.setMessageIndex(payload.getMessageIndex() + 1);
    }

    Utils::setMotors(payload, turn, speed);

    payload.print();

    payload.buildTransmission(buf, 7);

    for (int i = 0; i < sizeof(buf); i++)
    {
        Serial.println(buf[i], BIN);
    }

    raw_driver.send(buf, sizeof(buf));
    
    Serial.println();
}

void onPinActivated(int pinNr)
{
    payload.setStatus(1);
}

void onPinDeactivated(int pinNr)
{
    payload.setStatus(0);
}
