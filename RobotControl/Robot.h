#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <JsonEl.h>
#include "StatusLEDs.h"

#define ROBOT_TICK_DURATION_BUFFER_LEN 5

class Robot {
  static const unsigned long TICK_DURATION_MICROS;

  static const char *STATUS_DISABLED;
  static const char *STATUS_ENABLED;
  static const char *STATUS_PRIMED;

  public:
    Robot(JsonState &state, int pinLedBuiltin);

    void init();

    void update();

  private:
    void updateSerial(int ticks);
    void updateFromJson(const char *json);
    int getAverageTickDuration();
    void updateTickDurations(int tickDurationMicros);

    JsonState &m_state;
    StatusLEDs m_statusLEDs;
    int m_initTimeSeconds;
    int m_tickDurations[ROBOT_TICK_DURATION_BUFFER_LEN];
    size_t m_tickDurationsIndex;
};

#endif // ROBOT_H
