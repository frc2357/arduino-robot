#include "Robot.h"
#include "Utils.h"

const unsigned long Robot::TICK_DURATION_MILLIS = 100;
static const uint8_t Robot::SERIAL_PAYLOAD_LENGTH = 9;

static const uint8_t Robot::STATUS_DISABLED = 0;
static const uint8_t Robot::STATUS_ENABLED = 1;
static const uint8_t Robot::STATUS_PRIMED = 3;

Robot::Robot(TShirtCannonPayload &payload, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress) :
  m_payload(payload),
  m_statusLEDs(pinLedBuiltin),
  m_commsI2C(i2cHostAddress, i2cDeviceAddress)
{
  m_initTimeSeconds = 0;
}

void Robot::init() {
  m_initTimeSeconds = (int)(millis() / 1000);

  m_tickDurationsIndex = 0;
  for (int i = 0; i < ROBOT_TICK_DURATION_BUFFER_LEN; i++) {
    m_tickDurations[i] = 0;
  }

  m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
  m_commsI2C.init();
}

void Robot::update() {
  int tick = m_payload.getMessageIndex();
  unsigned long tickStartMillis = millis();

  m_statusLEDs.update(tick);

  uint8_t* transmission;
  // TODO: SWITCH OUT WITH PAYLOAD METHODS
  *transmission = 23;
  *(transmission + 1) = 57;
  m_payload.buildTransmission((transmission+2), 7);
  m_commsI2C.sendBytes(transmission, 9);

  // Increment time payload variables

  if(tick >= 31) {
    tick = 0;
  } else {
    tick++;
  }
  m_payload.setMessageIndex(tick);

  int tickDurationMillis = millis() - tickStartMillis;
  // TODO: Remove after timing is solved
  Serial.println(tickDurationMillis);
  updateTickDurations(tickDurationMillis);

  updateSerial();

  if (tickDurationMillis > TICK_DURATION_MILLIS) {
    setError("Tick %d ms", tickDurationMillis);
  }

  int timeLeftMillis = TICK_DURATION_MILLIS - (millis() - tickStartMillis);
  if (timeLeftMillis > 0) {
    delay(timeLeftMillis);
  }
}

void Robot::updateSerial() {
  if (Serial.available() >= 0) {

    
    uint8_t *data = Serial.read();
    updatePayload(data, SERIAL_PAYLOAD_LENGTH);
    m_payload.print();
    Serial.println();
  }
}

void Robot::updatePayload(const uint8_t *data, const uint8_t len) {
  bool success = m_payload.readMessage(data, len);

  const char *status = m_payload.getStatus();
  const char *err = m_payload.getStatus();

  if (strlen(err) > 0 || success) {
    m_statusLEDs.setBlinkPattern(StatusLEDs::ERROR);
    m_payload.setStatus(STATUS_DISABLED);
  } else {
    if (status == STATUS_DISABLED) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
    } else if (status == STATUS_ENABLED) {
      m_statusLEDs.setBlinkPattern(StatusLEDs::ENABLED);
    } else if (status == STATUS_PRIMED) {
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

void Robot::updateTickDurations(int tickDurationMillis) {
  m_tickDurations[m_tickDurationsIndex] = tickDurationMillis;
  m_tickDurationsIndex = Utils::incrementRingBufferIndex(m_tickDurationsIndex, ROBOT_TICK_DURATION_BUFFER_LEN);
}

void Robot::setError(const char *format, ...) {
  va_list args;
  char message[32];
  va_start(args, format);
  vsprintf(message, format, args);
  va_end(args);

  m_payload.setStatus(STATUS_DISABLED);

  Serial.print("ERROR: ");
  Serial.println(message);
}
