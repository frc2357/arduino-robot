#include "Robot.h"

const unsigned long Robot::TICK_DURATION_MILLIS = 100;
const uint8_t Robot::PREAMBLE_LEN = 4;
const unsigned int Robot::KEEP_ALIVE_MILLIS = 1000;

const uint8_t Robot::STATUS_DISABLED = 0;
const uint8_t Robot::STATUS_ENABLED = 1;
const uint8_t Robot::STATUS_ADJUSTING = 2;
const uint8_t Robot::STATUS_PRIMED = 3;
const uint8_t Robot::STATUS_FIRING = 4;

Robot::Robot(TShirtCannonPayload &payload, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress) :
  m_payload(payload),
  m_statusLEDs(pinLedBuiltin),
  m_commsI2C(i2cHostAddress, i2cDeviceAddress, PREAMBLE_LEN)
{
  m_initTimeSeconds = 0;
  m_lastRecvTimeMillis = 0;
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
  Serial.println("Printing");
  int tick = m_payload.getMessageIndex();
  unsigned long tickStartMillis = millis();

  m_statusLEDs.update(tick);

  // TODO: SWITCH OUT WITH PAYLOAD METHODS
  memset(m_payloadBytes, 0, PAYLOAD_LEN);
  m_payload.buildTransmission(m_payloadBytes, PAYLOAD_LEN);
  //m_commsI2C.sendBytes(m_payloadBytes, PAYLOAD_LEN);

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
  memset(m_payloadBytes, 0, PAYLOAD_LEN);
  memset(m_serialBuffer, 0, SERIAL_BUFFER_LEN);
  bool success = m_commsI2C.getBytes(m_serialBuffer, SERIAL_BUFFER_LEN, m_payloadBytes, PAYLOAD_LEN);

  if (success) {
    m_lastRecvTimeMillis = millis();
    updatePayload(m_payloadBytes, PAYLOAD_LEN);
    Serial.println("Bytes parsed");
  }

  setStatus();
  //setRobot();
  
  m_payload.print();
  Serial.println();
}

void Robot::updatePayload(const uint8_t *data, const uint8_t len) {
  bool success = m_payload.readMessage(data, len);

  const uint8_t status = m_payload.getStatus();
  const uint8_t err = m_payload.getStatus();

  if (err > 0 || success) {
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

void Robot::setRobot() {
   const uint8_t status = m_payload.getStatus();
  if (status != STATUS_ENABLED) {
    Serial.write((uint8_t)0);
    Serial.write((uint8_t)128);
  } else if (status == STATUS_ENABLED) {
    Serial.write(m_payload.getControllerDriveLeft());
    Serial.write(m_payload.getControllerDriveLeft());
  } 
}

void Robot::setStatus() {
  // TODO: Switch to using tick variable for keep alive
  if(millis() - m_lastRecvTimeMillis > KEEP_ALIVE_MILLIS) {
    m_payload.setStatus(STATUS_DISABLED);
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
