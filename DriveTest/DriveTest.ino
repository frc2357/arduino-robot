// ! IF USING THE CONTROLLER, MAKE SURE LINES 33-34 ARE NOT COMMENTED OUT

#include "JoystickAxis.h"
#include "Utils.h"
#include "TShirtCannonPayload.h"
#include "RFM95C.h"
#include "RHReliableDatagram.h"

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

RFM_95C raw_driver(RFM95_CS, RFM95_INT);

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
}

void loop()
{
    rightStick.update();
    leftStick.update();

    float turn, speed;
    turn = .5;
    speed = -1;
    turn = rightStick.getResult();
    speed = leftStick.getResult();

    Utils::setMotors(payload, turn, speed);

    Serial.print("Left motor speed: ");
    Serial.println(payload.getControllerDriveLeft());
    Serial.print("Right motor speed: ");
    Serial.println(payload.getControllerDriveRight());
    Serial.println();

    payload.buildTransmission(buf, 7);

    raw_driver.send(buf, sizeof(buf));
    int time = millis();

    raw_driver.waitPacketSent();

    Serial.println("Message sent in " + String(millis() - time));

    delay(1000);
}
