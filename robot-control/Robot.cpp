#include "Robot.h"

Robot::Robot(RobotPins pins) : statusLEDs(pins) {
  this->pins = pins;
  this->status = RobotStatus::STATUS_UNKNOWN;
}

RobotStatus Robot::GetStatus() {
  return this->status;
}

void Robot::Update(int loopCount) {
  this->statusLEDs.Update(this->status);
}

void Robot::Setup() {
  this->status = RobotStatus::STATUS_SETUP;
  this->statusLEDs.Setup();
}

void Robot::Disable() {
  this->status = RobotStatus::STATUS_DISABLED;
}

void Robot::Enable() {
  this->status = RobotStatus::STATUS_ENABLED;
}
