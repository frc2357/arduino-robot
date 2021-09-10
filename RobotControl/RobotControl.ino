/*
 * robot-control.ino
 * 
 * This is the main control program for the robot controller.
 * It can receive control information via i2c and then
 * controls the individual motors, solenoids, and devices.
 */

#include "RobotConstants.h"
#include "Robot.h"

int loopCount = 0;
RobotPins pins;
Robot robot(pins);

// Primary Setup
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);

  loopCount = 0;
  robot.Setup();
  robot.Disable();
}

// Main Loop
void loop() {
  robot.Update(loopCount);
  loopCount++;
}
