/*
 * RobotControl.ino
 * 
 * This is the main control program for the robot controller.
 * It can receive control information via i2c and then
 * controls the individual motors, solenoids, and devices.
 */

#include <TShirtCannonPayload.h>
#include "Utils.h"
#include "Robot.h"
#include "StatusLEDs.h"
#include "StatusDisabled.h"
#include "StatusEnabled.h"
#include "StatusAdjusting.h"
#include "StatusPrimed.h"
#include "StatusFiring.h"

#define SOFTWARE_VERSION   1
#define SERIAL_BAUD_RATE   115200
#define I2C_HOST_ADDRESS   0x09
#define I2C_DEVICE_ADDRESS 0x08
#define PIN_LED_BUILTIN    LED_BUILTIN
#define FIRE_SOLENOID_PIN 3

TShirtCannonPayload payload;

StatusDisabled disabled;
StatusEnabled enabled;
StatusAdjusting adjusting;
StatusPrimed primed;
StatusFiring firing;

Robot robot(payload, PIN_LED_BUILTIN, I2C_HOST_ADDRESS, I2C_DEVICE_ADDRESS, FIRE_SOLENOID_PIN
  disabled, enabled, adjusting, primed, firing);

// Primary Setup
void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  if (Serial) {
    //Serial.print("----------- RobotControl v");
    //Serial.print(SOFTWARE_VERSION);
    //Serial.println(" -----------");
  }
  robot.init();
  //Serial.print("----------- Init complete -----------");
}

// Main Loop
void loop() {
  robot.update();
  delay(100);
}
