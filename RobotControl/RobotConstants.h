#ifndef ROBOTCONSTANTS_H
#define ROBOTCONSTANTS_H

#define ROBOT_INITIAL_STATE "{ name: 'FRC2357 T-Shirt Cannon 2.0', status: 'unknown' }"

#define USB_BAUDRATE 115200

enum RobotStatus {
  STATUS_UNKNOWN = 0,
  STATUS_INIT = 1,
  STATUS_DISABLED = 2,
  STATUS_ENABLED = 3,
};

extern String RobotStatusText[];

class RobotPins {
  public:
    static const int ledBuiltin = LED_BUILTIN;
};

#endif // ROBOTCONSTANTS_H
