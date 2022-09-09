#include "Robot.h"

const unsigned long Robot::TICK_DURATION_MILLIS = 100;
const uint8_t Robot::PREAMBLE_LEN = 4;
const unsigned int Robot::KEEP_ALIVE_MILLIS = 100;

const unsigned long Robot::TEMP_FIRE_TIME_MILLIS = 100;

Robot::Robot(TShirtCannonPayload &payload, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress, int fireSolenoidPin,
  StatusDisabled &disabled, StatusEnabled &enabled, StatusAdjusting &adjusting, StatusPrimed &primed, StatusFiring &firing) :
  m_payload(payload),
  m_statusLEDs(pinLedBuiltin),
  m_commsI2C(i2cHostAddress, i2cDeviceAddress, PREAMBLE_LEN)
{
  m_initTimeSeconds = 0;
  m_lastRecvIndex = -1;
  m_lastRecvTimeMillis = 0;
  m_fireSolenoidPin = fireSolenoidPin;
  m_firing = false;
  m_isHoldingFire = false;

  m_currentStatus = STATUS_DISABLED;
  m_statuses[STATUS_DISABLED] = &disabled;
  m_statuses[STATUS_ENABLED] = &enabled;
  m_statuses[STATUS_ADJUSTING] = &adjusting;
  m_statuses[STATUS_PRIMED] = &primed;
  m_statuses[STATUS_FIRING] = &firing;
}

void Robot::init() {
  m_initTimeSeconds = (int)(millis() / 1000);

  m_tickDurationsIndex = 0;
  for (int i = 0; i < ROBOT_TICK_DURATION_BUFFER_LEN; i++) {
    m_tickDurations[i] = 0;
  }

  pinMode(m_fireSolenoidPin, OUTPUT);
  digitalWrite(m_fireSolenoidPin, LOW);

  m_statusLEDs.setBlinkPattern(StatusLEDs::DISABLED);
  m_commsI2C.init();
}

void Robot::update() {
  unsigned long tickStartMillis = millis();
  int tickDurationMillis = millis() - tickStartMillis;

  int tick = m_payload.getMessageIndex();

  m_statusLEDs.update(tick);

  // TODO: Remove after timing is solved
  // Serial.print("Tick time: ");
  // Serial.println(tickDurationMillis);
  updateTickDurations(tickDurationMillis);

  updateSerial();

  if (tickDurationMillis > TICK_DURATION_MILLIS) {
    setError("Tick %d ms", tickDurationMillis);
  }

  int timeLeftMillis = TICK_DURATION_MILLIS - (millis() - tickStartMillis);
  if (timeLeftMillis > 0) {
    //delay(timeLeftMillis);
  }
}

void Robot::transition(Status status) {
  m_currentStatus = status;
}

void Robot::updateSerial() {
  memset(m_payloadBytes, 0, PAYLOAD_LEN);
  memset(m_serialBuffer, 0, SERIAL_BUFFER_LEN);

  if (m_commsI2C.getBytes(m_serialBuffer, SERIAL_BUFFER_LEN, m_payloadBytes, PAYLOAD_LEN)) {
    updatePayload(m_payloadBytes, PAYLOAD_LEN);
    //Serial.println("Bytes parsed");
  }

  transition(static_cast<Status>(m_payload.getStatus()));

  m_statuses[m_currentStatus]->update();

  //m_payload.print();
  //Serial.println();
}

void Robot::updatePayload(const uint8_t *data, const uint8_t len) {
  bool success = m_payload.readMessage(data, len);

  const uint8_t status = m_payload.getStatus();
  const uint8_t err = m_payload.getError();

  if (err > 0 || !success) {
    m_statusLEDs.setBlinkPattern(StatusLEDs::ERROR);
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
  uint8_t status = m_payload.getStatus();

  if(m_firing) {
    if(millis() - TEMP_FIRE_TIME_MILLIS >= m_solenoidOpenMillis) {
      digitalWrite(m_fireSolenoidPin, LOW);
      m_firing = false;
      Serial.print("Open for: ");
      Serial.println(millis() - m_solenoidOpenMillis);
    }
  }

  if (status != STATUS_ENABLED) {
    Serial.write((uint8_t)0);
    Serial.write((uint8_t)128);
  } 

  if (status != STATUS_FIRING && status != STATUS_ADJUSTING) {
    digitalWrite(m_fireSolenoidPin, LOW);
    m_firing = false;
    m_isHoldingFire = false;
  }

  if (status == STATUS_ENABLED) {
    Serial.write(m_payload.getControllerDriveLeft());
    Serial.write(m_payload.getControllerDriveRight());
  } 

  if (status == STATUS_FIRING) {
    if(!m_isHoldingFire) {
      digitalWrite(m_fireSolenoidPin, HIGH);
      m_solenoidOpenMillis = millis();
      status = STATUS_ADJUSTING;
      Serial.println("Firing");
      m_firing = true;
      m_isHoldingFire = true;
    }
  }
  m_payload.setStatus(status);
}

void Robot::setStatus() {
  // First check if status should be Adjusting
  if(m_payload.getStatus() == STATUS_DISABLED) {
    return;
  }
  // Broken here
  if (m_firing) {
    if(millis() - TEMP_FIRE_TIME_MILLIS < m_solenoidOpenMillis) {
      m_payload.setStatus(STATUS_ADJUSTING);
      Serial.println("Forced adjusting");
    }
  }

  // Second check if status should be Disabled
  int currentIndex = m_payload.getMessageIndex();
  if (m_lastRecvIndex != currentIndex) {
    m_lastRecvTimeMillis = millis();
    m_lastRecvIndex = currentIndex;
  }

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

  //Serial.print("ERROR: ");
  //Serial.println(message);
}



void StatusDisabled::update() {
  Serial.println("HELP IM DISABLED");
  m_robot->m_payload.getStatus();

  validateState();
  robotAction();

  //m_robot->transition(Status::STATUS_ENABLED);
}

void StatusDisabled::validateState() {
    
}

void StatusDisabled::robotAction() {
    
}

void StatusEnabled::update() {
  validateState();
  robotAction();
}

void StatusEnabled::validateState() {
}

void StatusEnabled::robotAction() {
    
}

void StatusAdjusting::update() {
  validateState();
  robotAction();
}

void StatusAdjusting::validateState() {
    
}

void StatusAdjusting::robotAction() {
    
}

void StatusPrimed::update() {
  validateState();
  robotAction();
}

void StatusPrimed::validateState() {
    
}

void StatusPrimed::robotAction() {
    
}


void StatusFiring::update() {
  validateState();
  robotAction();
}

void StatusFiring::validateState() {
    
}

void StatusFiring::robotAction() {
    
}