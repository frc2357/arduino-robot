#ifndef STATUSLEDS_H
#define STATUSLEDS_H

#include <Arduino.h>
#include <JsonEl.h>

class StatusLEDs {
  public:
    enum BlinkPattern { OFF = 0, DISABLED = 1, ENABLED = 2, PRIMED = 3, ERROR = 4 };

    StatusLEDs(int pinBuiltinLed);

    void init();
    void update(int tick);
    BlinkPattern getBlinkPattern();
    void setBlinkPattern(BlinkPattern blinkPattern);

  private:
    static const int UPDATE_TICK_MODULUS;
    static const char *blinkPatternTiming[];

    int m_pinBuiltinLed;
    BlinkPattern m_blinkPattern;
    int m_blinkIndex;
};

#endif // STATUSLEDS_H
