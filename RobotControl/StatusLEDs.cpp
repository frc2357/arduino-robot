#include "StatusLEDs.h"
#include "Utils.h"

const int StatusLEDs::BLINK_TIMING_MILLIS = 100;

const char *StatusLEDs::blinkPatternTiming[] = {
  /* OFF */   "-",
  /* DISABLED */ "*",
  /* ENABLED */  "**********----------",
  /* PRIMED */  "*****-----",
  /* ERROR */  "*-*------",
};

StatusLEDs::StatusLEDs(int pinBuiltinLed) {
  m_pinBuiltinLed = pinBuiltinLed;
}

void StatusLEDs::init() {
  pinMode(m_pinBuiltinLed, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  m_lastPinValue = LOW;
  m_blinkPattern = StatusLEDs::BlinkPattern::OFF;
  m_blinkIndex = 0;
  m_lastBlinkTime = 0;
}

void StatusLEDs::update() {
  int time = millis();

  // If enough time has passed, get the next blink value.
  if (time - m_lastBlinkTime > BLINK_TIMING_MILLIS) {
    int pinValue = nextLEDValue();

    if (m_lastPinValue != pinValue) {
      digitalWrite(m_pinBuiltinLed, pinValue);
      m_lastPinValue = pinValue;
    }
    m_lastBlinkTime = time;
  }
}

StatusLEDs::BlinkPattern StatusLEDs::getBlinkPattern() {
  return m_blinkPattern;
}

void StatusLEDs::setBlinkPattern(BlinkPattern blinkPattern) {
  if (m_blinkPattern != blinkPattern) {
    m_blinkPattern = blinkPattern;
    m_blinkIndex = 0;
    m_lastBlinkTime = 0;
  }
}

int StatusLEDs::nextLEDValue() {
  const char *blinkTiming = StatusLEDs::blinkPatternTiming[m_blinkPattern];
  int value = blinkTiming[m_blinkIndex] == '*' ? HIGH : LOW;

  // Increment blinkIndex, but loop around to beginning.
  m_blinkIndex = Utils::incrementRingBufferIndex(m_blinkIndex, strlen(blinkTiming) - 1);
  return value;
}
