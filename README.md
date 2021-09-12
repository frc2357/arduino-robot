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

## JSON State

There's a JSON State object that represents the current state of the robot,
and this is how updates are sent from the controller as well.

### Full status of the robot with examples and explanations:

```
{
  "version": "1.0.0",       // Software version of the robot
  "status": "Enabled",      // Current status of robot ["Disabled", "Enabled", "Primed"]
  "humanControls": {        // Info about connection to human controls
    "id": 123,              // Id of the human controller
    "connection": "Active", // ["Disconnected", "Active"]
  },
  "eStop": {                // List of possible e-stop buttons
    "onBoard": false,       // Wired in to the robot's arduino
  },
  "batteryLevel": 0.95      // 0.0=dead, 1.0=full charge
  "barrelAngle": 0.5,       // 0.0=lowest, 1.0=highest
  "shotPressure": 25.2,     // Current shot tank pressure (in PSI)
  "firePressure": 30.5,     // Target shot tank pressure to fire (in PSI)
  "valveDuration": 110,     // Amount of time the valve opens for firing a shot
  "driveVelocity": 0.5,     // -1.0=full reverse, 1.0=full ahead
  "driveTurn": 0.25,        // -1.0=full counter-clockwise, 1.0=full clockwise
  "firing": false,          // Controller sets to true to fire, robot clears to false afterward
  "filling": false,         // True if the shot tank is filling up
  "error": "it broke",      // error message or "" if none (max length: 32)
}
```
