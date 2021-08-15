#include <JsonState.h>
#include "RobotConstants.h"
#include "Robot.h"

String RobotStatusText[] = {
  "unknown",
  "init",
  "disabled",
  "enabled",
};

Robot::Robot(RobotPins pins) : statusLEDs(pins), jsonState(ROBOT_INITIAL_STATE, 1024) {
  this->pins = pins;
  this->status = RobotStatus::STATUS_UNKNOWN;
}

RobotStatus Robot::GetStatus() {
  return this->status;
}

void Robot::Init() {
  this->status = RobotStatus::STATUS_INIT;
  this->jsonState.updateField(this->jsonState.getState(), "status", RobotStatusText[this->status]);
  this->statusLEDs.Init();
}

void Robot::Update() {
  this->jsonState.writeToSerial(true);
  Serial.println();

  if (Serial.available() > 0) {
    String fromSerial = Serial.readString();
    Serial.println("fromSerial:");
    Serial.println(fromSerial);
    this->jsonState.updateFromString(fromSerial);
    /*
    String result = this->jsonState.updateFromSerial();
    if (result != JsonState::Ok) {
      Serial.println("Serial Error:");
      Serial.println(result);
    }
    */
  }

  this->statusLEDs.Update(this->status);
}

void Robot::Disable() {
  this->status = RobotStatus::STATUS_DISABLED;
}

void Robot::Enable() {
  this->status = RobotStatus::STATUS_ENABLED;
}
