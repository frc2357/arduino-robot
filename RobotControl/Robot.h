#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <TShirtCannonPayload.h>
#include <CommsI2CMaster.h>
#include "StatusLEDs.h"
#include "Utils.h"
#include "StatusEnum.h"
#include "RobotStatus.h"

#define ROBOT_TICK_DURATION_BUFFER_LEN 5
#define PAYLOAD_LEN 7
#define SERIAL_BUFFER_LEN 50
#define NUM_STATUSES 5

class StatusDisabled;
class StatusEnabled;
class StatusAdjusting;
class StatusPrimed;
class StatusFiring;

class Robot
{
  static const unsigned long TICK_DURATION_MILLIS;
  static const uint8_t PREAMBLE_LEN;
  static const unsigned int KEEP_ALIVE_MILLIS;

  static const unsigned long TEMP_FIRE_TIME_MILLIS;

  friend StatusDisabled;
  friend StatusEnabled;
  friend StatusAdjusting;
  friend StatusPrimed;
  friend StatusFiring;

public:
  Robot(TShirtCannonPayload &payload, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress, int fireSolenoidPin,
    StatusDisabled &disabled, StatusEnabled &enabled, StatusAdjusting &adjusting, StatusPrimed &primed, StatusFiring &firing);

  void init();
  void update();
  void transition(Status status);

private:
  void updateSerial();
  void updatePayload(const uint8_t *data, const uint8_t len);
  int getAverageTickDuration();
  void updateTickDurations(int tickDurationMicros);
  int binToPWM(uint8_t value);
  void setError(const char *format, ...);
  void setDrive();
  void stopDriving();
  void fire();
  void stopFiring();
  void keepAlive();
  void isFiring();

  TShirtCannonPayload &m_payload;
  StatusLEDs m_statusLEDs;
  CommsI2CMaster m_commsI2C;
  LinearActuator m_actuator;
  
  int m_initTimeSeconds;
  int m_tickDurations[ROBOT_TICK_DURATION_BUFFER_LEN];
  size_t m_tickDurationsIndex;
  uint8_t m_lastRecvIndex;
  unsigned long m_lastRecvTimeMillis;

  uint8_t m_payloadBytes[PAYLOAD_LEN];
  uint8_t m_serialBuffer[SERIAL_BUFFER_LEN];

  int m_fireSolenoidPin;

  int m_leftDrivePWM;

  int m_rightDrivePWM;

  bool m_firing;
  bool m_isHoldingFire;

  RobotStatus *m_statuses[NUM_STATUSES];
  Status m_currentStatus;
};

class StatusDisabled : public RobotStatus {
public:
    void update();
    void validateState();
};

class StatusEnabled : public RobotStatus {
public:
    void update();
    void validateState();
};

class StatusAdjusting : public RobotStatus {
public:
    void update();
    void validateState();
};

class StatusPrimed : public RobotStatus {
public:
    void update();
    void validateState();
};

class StatusFiring : public RobotStatus {
public:
    void update();
    void validateState();
};

#endif // ROBOT_H