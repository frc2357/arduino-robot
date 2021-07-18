#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SERIAL_BAUD_RATE 115200

enum RobotStatus {
  STATUS_UNKNOWN = 0,
  STATUS_SETUP = 1,
  STATUS_DISABLED = 2,
  STATUS_ENABLED = 3,
};

class RobotPins {
  public:
    static const int ledBuiltin = LED_BUILTIN;
};

#endif // CONSTANTS_H
