#include "Robot.h"
#include "Utils.h"

const unsigned long Robot::TICK_DURATION_MICROS = 10000;
const unsigned long Robot::I2C_UPDATE_TICKS = 100;

const char *Robot::STATUS_DISABLED = "Disabled";
const char *Robot::STATUS_ENABLED = "Enabled";
const char *Robot::STATUS_PRIMED = "Primed";

Robot::Robot(JsonState &state, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress) :
  m_state(state),
  m_statusLEDs(pinLedBuiltin),
  m_commsI2C(i2cHostAddress, i2cDeviceAddress)
{
  m_initTimeSeconds = 0;
}

void Robot::init() {
  m_initTimeSeconds = (int)(millis() / 1000);

  m_tickDurationsIndex = 0;
  for (int i = 0; i < ROBOT_TICK_DURATION_BUFFER_LEN; i++) {
    m_tickDurations[i] = -1;
  }

  m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
  m_commsI2C.init();
}

void Robot::update() {
  int tick = m_state.root()["tck"].asInt();
  unsigned long tickStartMicros = micros();

  m_statusLEDs.update(tick);

  if (tick % I2C_UPDATE_TICKS == 0) {
    m_commsI2C.sendState(m_state);
  }

  // Increment time state variables
  m_state.root()["tck"] = tick + 1;
  m_state.root()["up"] = (int)((tickStartMicros / 1000000) - m_initTimeSeconds);
  m_state.root()["avgTck"] = getAverageTickDuration();

  long tickDurationMicros = micros() - tickStartMicros;
  updateTickDurations((int)tickDurationMicros);

  if (tickDurationMicros > TICK_DURATION_MICROS) {
    setError("Tick overflow %ld us", tickDurationMicros);
  }

  // Updating from Serial can take longer than a tick.
  // So this is outside of the overflow logging.
  updateSerial();

  int timeLeftMicros = TICK_DURATION_MICROS - (micros() - tickStartMicros);
  if (timeLeftMicros > 0) {
    delay(timeLeftMicros / 1000);
  }
}

void Robot::updateSerial() {
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
    m_state.root()["status"] = STATUS_DISABLED;
  } else {
    if (strcmp(status, STATUS_DISABLED) == 0) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
    } else if (strcmp(status, STATUS_ENABLED) == 0) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::ENABLED);
    } else if (strcmp(status, STATUS_PRIMED) == 0) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::PRIMED);
    } else {
      m_statusLEDs.setBlinkPattern(StatusLEDs::OFF);
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

void Robot::setError(const char *format, ...) {
  va_list args;
  char message[32];
  va_start(args, format);
  vsprintf(message, format, args);
  va_end(args);

  m_state.root()["err"] = message;
  m_state.root()["status"] = STATUS_DISABLED;

  Serial.print("ERROR: ");
  Serial.println(message);
}
