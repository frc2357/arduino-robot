/*
 * RobotControl.ino
 * 
 * This is the main control program for the robot controller.
 * It can receive control information via i2c and then
 * controls the individual motors, solenoids, and devices.
 */

#include <JsonElement.h>
#include "Utils.h"
#include "Robot.h"
#include "StatusLEDs.h"

#define SOFTWARE_VERSION   1
#define SERIAL_BAUD_RATE   115200
#define I2C_HOST_ADDRESS   0x04
#define I2C_DEVICE_ADDRESS 0x08
#define PIN_LED_BUILTIN    LED_BUILTIN

// Set up the JSON State for the robot
JsonElement eStopFields[] = {
  Json::Boolean("btn", true),
};

JsonElement robotStateFields[] = {
  Json::Int("v", SOFTWARE_VERSION),
  Json::Int("t", 0),
  Json::String("status", "Disabled"),
  Json::Boolean("hCtl", false),
  Json::Object("eStop", eStopFields),
  Json::Float("b", 0.0),
  Json::Int("ang", 0),
  Json::Int("tPr", 0),
  Json::Int("fPr", 0),
  Json::Int("vlv", 0),
  Json::Float("x", 0.0),
  Json::Float("y", 0.0),
  Json::Boolean("fire", false),
  Json::Boolean("fill", false),
  Json::String("err", "", 16),
};
JsonElement robotState = Json::Object(robotStateFields);

JsonState state(robotState);

Robot robot(state, PIN_LED_BUILTIN, I2C_HOST_ADDRESS, I2C_DEVICE_ADDRESS);

// Primary Setup
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  if (Serial) {
    Serial.print("----------- RobotControl v");
    Serial.print(SOFTWARE_VERSION);
    Serial.println(" -----------");
  }
  robot.init();
  Serial.print("----------- Init complete -----------");
}

// Main Loop
void loop() {
  robot.update();
}
