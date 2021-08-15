#ifndef STATUSLEDS_H
#define STATUSLEDS_H

#include "Arduino.h"
#include "RobotConstants.h"

class StatusLEDs {
  public:
    StatusLEDs(RobotPins pins);

    void Init();

    void Update(RobotStatus status);

  private:
    const static int BLINK_TIMING_MILLIS = 100;
    enum BlinkPattern { OFF = 0, DISABLED = 1, ENABLED = 2, ERROR = 3 };
    const static String blinkPatternTiming[];

    static BlinkPattern GetBlinkPattern(RobotStatus status);

    void SetBlinkPattern(BlinkPattern blinkPattern);
    int NextLEDValue();

    int pinBuiltin;
    BlinkPattern blinkPattern;
    int blinkIndex;
    unsigned long lastBlinkTime;
    int lastPinValue;
    RobotStatus lastStatus;
};

#endif // STATUSLEDS_H
