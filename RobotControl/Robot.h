#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <JsonElement.h>
#include "StatusLEDs.h"
#include "CommsI2C.h"

#define ROBOT_TICK_DURATION_BUFFER_LEN 5

class Robot {
  static const unsigned long TICK_DURATION_MILLIS;

  static const char *STATUS_DISABLED;
  static const char *STATUS_ENABLED;
  static const char *STATUS_PRIMED;

  public:
    Robot(JsonState &state, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress);

    void init();
    void update();

  private:
    void updateSerial();
    void updateFromJson(const char *json);
    int getAverageTickDuration();
    void updateTickDurations(int tickDurationMicros);
    void setError(const char *format, ...);

    JsonState &m_state;
    StatusLEDs m_statusLEDs;
    CommsI2C m_commsI2C;
    int m_initTimeSeconds;
    int m_tickDurations[ROBOT_TICK_DURATION_BUFFER_LEN];
    size_t m_tickDurationsIndex;
};

#endif // ROBOT_H
