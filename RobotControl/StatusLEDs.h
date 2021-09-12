#ifndef STATUSLEDS_H
#define STATUSLEDS_H

#include <Arduino.h>
#include <JsonEl.h>

class StatusLEDs {
  public:
    enum BlinkPattern { OFF = 0, DISABLED = 1, ENABLED = 2, PRIMED = 3, ERROR = 4 };

    StatusLEDs(int pinBuiltinLed);

    void init();
    void update();
    BlinkPattern getBlinkPattern();
    void setBlinkPattern(BlinkPattern blinkPattern);

  private:
    static const int BLINK_TIMING_MILLIS;
    static const char *blinkPatternTiming[];

    int nextLEDValue();

    int m_pinBuiltinLed;
    BlinkPattern m_blinkPattern;
    int m_blinkIndex;
    unsigned long m_lastBlinkTime;
    int m_lastPinValue;
};

#endif // STATUSLEDS_H
