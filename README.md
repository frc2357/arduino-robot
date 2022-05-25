# arduino-robot

This is a project to control a t-shirt cannon robot using 3 Arduinos:

1. Robot control Arduino
2. Robot comms Arduino (900 MHz)
3. Human Controller Arduino (900 MHz)

## To Build

### Prerequisites

1. Download and install NodeJS: https://nodejs.org/en/
2. Download and install Arduino IDE: https://www.arduino.cc/en/software
3. Download and install `arduino-cli`: https://arduino.github.io/arduino-cli/latest/installation/
4. Download and install Visual Studio Code: https://code.visualstudio.com/
5. Install Arduino extension for VSCode: https://marketplace.visualstudio.com/items?itemName=vsciot-vscode.vscode-arduino
6. Clone this repository

### To build individual project

## Binary State Object

In order to keep radio messages as small as possible, we are using a binary packed C Struct.

Full status messages are sent regularly from the robot to the controller.
The robot is considered the single source of truth for the full state.
The controller can send a full state as well, and the robot will attempt to change its
state to match the desired state from the controller, then return a full state message to confirm.

```
{
  uint:2 message_type;           // 0=From robot, 1=From controller, 2=no op from controller, 3=drive x/y from controller
  uint:5 message_index;          // Count up from 0, then roll over from 31 back to 0
  uint:3 status;                 // 0=disabled, 1=enabled, 2=adjusting (cannot prime or fire), 3=primed, 4=firing
  uint:2 error;                  // 0=no error, 1=e-stop, 2=no controller, 3=other (check logs)
  uint:8 controller_drive_left;  // Specific to motor controller
  uint:8 controller_drive_left;  // Specific to motor controller
  uint:2 battery_voltage;        // 0= <20, 1= 20-22, 2= 22-24,3= 24+
  uint:7 angle;                  // 0=down, 127=up
  uint:7 tank_pressure;          // In PSI: 0-127
  uint:7 firing_pressure;        // In PSI: 0-127
  uint:5 firing_time;            // 0=100ms, 20=300ms
}
```
