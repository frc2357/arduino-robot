/*
 * RobotControl.ino
 * 
 * This is the main control program for the robot controller.
 * It can receive control information via i2c and then
 * controls the individual motors, solenoids, and devices.
 */

#include <JsonEl.h>
#include "Utils.h"
#include "Robot.h"
#include "StatusLEDs.h"

#define SERIAL_BAUD_RATE   115200
#define I2C_HOST_ADDRESS   0x04
#define I2C_DEVICE_ADDRESS 0x08
#define PIN_LED_BUILTIN    LED_BUILTIN

// Set up the JSON State for the robot
JsonElement humanControlFields[] = {
  Json::Int("id", -1),
  Json::String("conn", "Disconnected"),
};

JsonElement eStopFields[] = {
  Json::Boolean("btn", true),
};

JsonElement robotStateFields[] = {
  Json::String("ver", "1.0.0"),
  Json::Int("tck", 0),
  Json::Int("avgTck", 0),
  Json::Int("up", 0),
  Json::String("status", "Disabled"),
  Json::Object("hCtrl", humanControlFields),
  Json::Object("eStop", eStopFields),
  Json::Float("bat", 0.0),
  Json::Float("angle", 0.0),
  Json::Float("tnkPres", 0.0),
  Json::Float("frPres", 0.0),
  Json::Int("vlvTm", 0),
  Json::Float("dVel", 0.0),
  Json::Float("dRot", 0.0),
  Json::Boolean("fire", false),
  Json::Boolean("fill", false),
  Json::String("err", "", 32),
};
JsonElement robotState = Json::Object(robotStateFields);

JsonState state(robotState);

Robot robot(state, PIN_LED_BUILTIN, I2C_HOST_ADDRESS, I2C_DEVICE_ADDRESS);

// Primary Setup
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  robot.init();
}

// Main Loop
void loop() {
  robot.update();
}
