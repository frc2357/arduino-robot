/*
 * robot-control.ino
 * 
 * This is the main control program for the robot controller.
 * It can receive control information via i2c and then
 * controls the individual motors, solenoids, and devices.
 */

#include <JsonState.h>
#include "RobotConstants.h"
#include "StatusLEDs.h"
#include "Robot.h"

RobotPins pins;
Robot robot(pins);

// Primary Setup
void setup() {
  Serial.begin(USB_BAUDRATE);
  robot.Init();
}

// Main Loop
void loop() {
  robot.Update();

  //double count = jsonState.getDoubleValue(exampleDevice, "count");
  //jsonState.updateField(exampleDevice, "count", count + 0.5);
  delay(500);

  robot.Disable();
}
