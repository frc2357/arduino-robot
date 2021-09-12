#include "Robot.h"
#include "Log.h"
#include "Utils.h"

const unsigned long Robot::TICK_DURATION_MICROS = 10000;

const char *Robot::STATUS_DISABLED = "Disabled";
const char *Robot::STATUS_ENABLED = "Enabled";
const char *Robot::STATUS_PRIMED = "Primed";

Robot::Robot(JsonState &state, int pinLedBuiltin) : m_state(state), m_statusLEDs(pinLedBuiltin) {
  m_initTimeSeconds = 0;
}

void Robot::init() {
  m_initTimeSeconds = (int)(millis() / 1000);

  m_tickDurationsIndex = 0;
  for (int i = 0; i < ROBOT_TICK_DURATION_BUFFER_LEN; i++) {
    m_tickDurations[i] = -1;
  }

  m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
}

void Robot::update() {
  int tick = m_state.root()["tck"].asInt();
  unsigned long tickStartMicros = micros();

  m_statusLEDs.update();

  m_state.root()["tck"] = tick + 1;
  m_state.root()["up"] = (int)((tickStartMicros / 1000000) - m_initTimeSeconds);
  m_state.root()["avgTck"] = getAverageTickDuration();

  int tickDurationMicros = (int)(micros() - tickStartMicros);
  updateTickDurations(tickDurationMicros);

  if (tickDurationMicros > TICK_DURATION_MICROS) {
    Log::error("Tick overflow %d us", tickDurationMicros - TICK_DURATION_MICROS);
  }

  updateSerial(tick);

  int timeLeftMicros = TICK_DURATION_MICROS - (micros() - tickStartMicros);
  if (timeLeftMicros > 0) {
    delay(timeLeftMicros / 1000);
  }
}

void Robot::updateSerial(int tick) {
  if (Serial.available() > 0) {
    String line = Serial.readString();
    updateFromJson(line.c_str());
    m_state.printJson(Serial);
    Serial.println();
  }
}

void Robot::updateFromJson(const char *json) {
  m_state.updateFromJson(json);

  const char *status = m_state.root()["status"].asString();
  const char *err = m_state.root()["err"].asString();

  if (strlen(err) > 0) {
    m_statusLEDs.setBlinkPattern(StatusLEDs::ERROR);
  } else {
    if (strcmp(status, STATUS_ENABLED) == 0) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
    } else if (strcmp(status, STATUS_DISABLED) == 0) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::ENABLED);
    } else if (strcmp(status, STATUS_PRIMED) == 0) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::PRIMED);
    }
  }
}

int Robot::getAverageTickDuration() {
  unsigned long total = 0;
  for (int i = 0; i < ROBOT_TICK_DURATION_BUFFER_LEN; i++) {
    total += m_tickDurations[i];
  }
  return (int)(total / ROBOT_TICK_DURATION_BUFFER_LEN);
}

void Robot::updateTickDurations(int tickDurationMicros) {
  m_tickDurations[m_tickDurationsIndex] = tickDurationMicros;
  m_tickDurationsIndex = Utils::incrementRingBufferIndex(m_tickDurationsIndex, ROBOT_TICK_DURATION_BUFFER_LEN);
}
