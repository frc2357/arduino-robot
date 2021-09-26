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
  "ver": "1.0.0",          // Software version of the robot
  "tck": 123,              // The current "tick" loop count
  "avgTck": 32,            // The time in milliseconds of the current average tick duration
  "up": 1193,              // The number of seconds since the robot has booted up
  "status": "Enabled",     // Current status of robot ["Disabled", "Enabled", "Primed"]
  "hCtrl": {               // Info about connection to human controls
    "id": 123,             // Id of the human controller
    "conn": "Active",      // ["Disconnected", "Active"]
  },
  "eStop": {               // List of possible e-stop buttons
    "btn": false,          // Wired in to the robot's arduino
  },
  "bat": 0.95              // 0.0=dead, 1.0=full charge
  "angle": 0.5,            // 0.0=lowest, 1.0=highest
  "tnkPres": 25.2,         // Current shot tank pressure (in PSI)
  "frPres": 30.5,          // Target shot tank pressure to fire (in PSI)
  "vlvTm": 110,            // Amount of time the valve opens for firing a shot
  "dVel": 0.5,             // -1.0=full reverse, 1.0=full ahead
  "dRot": 0.25,            // -1.0=full counter-clockwise, 1.0=full clockwise
  "fire": false,           // Controller sets to true to fire, robot clears to false afterward
  "fill": false,           // True if the shot tank is filling up
  "err": "it broke",       // error message or "" if none (max length: 32)
}
```
