#ifndef ROBOT_H
#define ROBOT_H

#include <Arduino.h>
#include <TShirtCannonPayload.h>
#include <CommsI2C.h>
#include "StatusLEDs.h"
#include "Utils.h"

#define ROBOT_TICK_DURATION_BUFFER_LEN 5
#define PAYLOAD_LEN 7
#define SERIAL_BUFFER_LEN 50

class Robot
{
  static const unsigned long TICK_DURATION_MILLIS;
  static const uint8_t PREAMBLE_LEN;

  static const uint8_t STATUS_DISABLED;
  static const uint8_t STATUS_ENABLED;
  static const uint8_t STATUS_PRIMED;

public:
  Robot(TShirtCannonPayload &payload, int pinLedBuiltin, int i2cHostAddress, int i2cDeviceAddress);

  void init();
  void update();

private:
  void updateSerial();
  void updatePayload(const uint8_t *data, const uint8_t len);
  int getAverageTickDuration();
  void updateTickDurations(int tickDurationMicros);
  void setError(const char *format, ...);

  TShirtCannonPayload &m_payload;
  StatusLEDs m_statusLEDs;
  CommsI2C m_commsI2C;
  int m_initTimeSeconds;
  int m_tickDurations[ROBOT_TICK_DURATION_BUFFER_LEN];
  size_t m_tickDurationsIndex;

  uint8_t m_payloadBytes[PAYLOAD_LEN];
  uint8_t m_serialBuffer[SERIAL_BUFFER_LEN];
};

#endif // ROBOT_H
