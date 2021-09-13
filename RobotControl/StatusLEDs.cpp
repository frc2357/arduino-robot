#include "StatusLEDs.h"
#include "Utils.h"

const int StatusLEDs::UPDATE_TICK_MODULUS = 10;

const char *StatusLEDs::blinkPatternTiming[] = {
  /* OFF */      "-",
  /* DISABLED */ "*",
  /* ENABLED */  "------******",
  /* PRIMED */   "--**",
  /* ERROR */    "-*-*--------",
};

StatusLEDs::StatusLEDs(int pinBuiltinLed) {
  m_pinBuiltinLed = pinBuiltinLed;
}

void StatusLEDs::init() {
  pinMode(m_pinBuiltinLed, OUTPUT);
  m_blinkPattern = StatusLEDs::BlinkPattern::OFF;
  m_blinkIndex = 0;
}

void StatusLEDs::update(int tick) {
  if (tick % UPDATE_TICK_MODULUS == 0) {
    const char *blinkTiming = StatusLEDs::blinkPatternTiming[m_blinkPattern];
    int value = blinkTiming[m_blinkIndex] == '*' ? HIGH : LOW;

    digitalWrite(m_pinBuiltinLed, value);

    m_blinkIndex = Utils::incrementRingBufferIndex(m_blinkIndex, strlen(blinkTiming));
  }
}

StatusLEDs::BlinkPattern StatusLEDs::getBlinkPattern() {
  return m_blinkPattern;
}

void StatusLEDs::setBlinkPattern(BlinkPattern blinkPattern) {
  if (m_blinkPattern != blinkPattern) {
    m_blinkPattern = blinkPattern;
    m_blinkIndex = 0;
  }
}
