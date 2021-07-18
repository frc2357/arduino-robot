#include "StatusLEDs.h"

const String StatusLEDs::blinkPatternTiming[] = {
  /* OFF */   "-",
  /* DISABLED */ "*",
  /* ENABLED */  "**********----------",
  /* ERROR */  "*-*------",
};

StatusLEDs::StatusLEDs(RobotPins pins) {
  this->pinBuiltin = pins.ledBuiltin;
}

void StatusLEDs::Setup() {
  pinMode(this->pinBuiltin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  this->lastPinValue = LOW;
  this->blinkPattern = StatusLEDs::BlinkPattern::OFF;
  this->blinkIndex = 0;
  this->lastStatus = RobotStatus::STATUS_UNKNOWN;
  this->lastBlinkTime = 0;
}

void StatusLEDs::Update(RobotStatus status) {
  int time = millis();

  // Check if the status has changed, and update if so.
  this->SetBlinkPattern(this->GetBlinkPattern(status));

  // If enough time has passed, get the next blink value.
  if (time - this->lastBlinkTime > BLINK_TIMING_MILLIS) {
    int pinValue = this->NextLEDValue();

    if (this->lastPinValue != pinValue) {
      digitalWrite(this->pinBuiltin, pinValue);
      this->lastPinValue = pinValue;
    }
    this->lastBlinkTime = time;
  }
}

StatusLEDs::BlinkPattern StatusLEDs::GetBlinkPattern(RobotStatus status) {
  switch(status) {
    case RobotStatus::STATUS_DISABLED:
      return StatusLEDs::BlinkPattern::DISABLED;
    case RobotStatus::STATUS_ENABLED:
      return StatusLEDs::BlinkPattern::ENABLED;
    case RobotStatus::STATUS_SETUP:
      return StatusLEDs::BlinkPattern::OFF;
    default:
      return StatusLEDs::BlinkPattern::OFF;
  }
}

void StatusLEDs::SetBlinkPattern(BlinkPattern blinkPattern) {
  if (this->blinkPattern != blinkPattern) {
    this->blinkPattern = blinkPattern;
    this->blinkIndex = 0;
    this->lastBlinkTime = 0;
  }
}

int StatusLEDs::NextLEDValue() {
  String blinkTiming = StatusLEDs::blinkPatternTiming[this->blinkPattern];
  int value = blinkTiming.charAt(this->blinkIndex) == '*' ? HIGH : LOW;

  // Increment blinkIndex, but loop around to beginning.
  this->blinkIndex = this->blinkIndex < (blinkTiming.length() - 1) ? this->blinkIndex + 1 : 0;
  return value;
}
