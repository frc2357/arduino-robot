#include "JoystickAxis.h"
#include "Utils.h"
#include "TShirtCannonPayload.h"

#define JOYSTICK_PIN_VRX 0
#define JOYSTICK_PIN_VRY 1

#define DEAD_ZONE_SIZE 100
#define JOYSTICK_MAX 1023

TShirtCannonPayload payload;

JoystickAxis leftStick(JOYSTICK_PIN_VRY, DEAD_ZONE_SIZE, JOYSTICK_MAX);
JoystickAxis rightStick(JOYSTICK_PIN_VRX, DEAD_ZONE_SIZE, JOYSTICK_MAX);

void setup()
{
    Serial.begin(115200);
}

void loop()
{
    rightStick.update();
    leftStick.update();

    float turn, speed;
    turn = rightStick.getResult();
    speed = leftStick.getResult();

    Utils::setMotors(payload, turn, speed);

    Serial.print("Left motor speed: ");
    Serial.println(payload.getControllerDriveLeft());
    Serial.print("Right motor speed: ");
    Serial.println(payload.getControllerDriveRight());
    Serial.println();
}
