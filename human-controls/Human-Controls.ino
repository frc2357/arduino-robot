#include "Arduino.h"
#include "FTDebouncer.h"
#include "src/RotaryKnobController.h"
#include "src/CharacterDisplay.h"
#include "src/MenuController.h"
#include "src/DisplayController.h"
#include "src/DisplayPage.h"
#include "src/Printer.h"
#include "src/JoyStickController.h"

//Pins
//LCD connects SDA in analog pin 4 and SCL in analog pin 5
#define ENCODER_PIN_CLK 3  //CLK gets degrees for rotary knob
#define ENCODER_PIN_DT 4   //DT gets direction for rotary knob
#define ENCODER_PIN_SW 5   //Gets the button for rotary knob
#define JOYSTICK_PIN_SW 6  //Button for joystick
#define JOYSTICK_PIN_VRX 0 //Analog Pin for joystick x
#define JOYSTICK_PIN_VRY 1 //Analog Pin for joystick y

//Other constraints
#define DISPLAY_ADDRESS 0X27 //I2c address of the lcd display
#define DISPLAY_LENGTH 16    //Length of the lcd display
#define DISPLAY_WIDTH 2      //width of the lcd display
#define MENU_SIZE 4          //Number of menu pages
#define USB_BAUDRATE 115200

//Joystick deadzones
#define X_DEAD_ZONE_SIZE 100 //Total size of the x deadzone
#define X_MIN 0
#define X_MAX 1023
#define Y_DEAD_ZONE_SIZE 100 //Total size of the y deadzone
#define Y_MIN 0
#define Y_MAX 1023

//Lengths of printers inside each DisplayPage
#define DASH_PRINTER_LEN 1
#define ELEVATOR_PRINTER_LEN 4
#define SHOT_PRINTER_LEN 4
#define VALVE_PRINTER_LEN 4

//Min - Max FOR sensors
#define ANGLE_MIN 0      //Minimum elevator angle
#define ANGLE_MAX 10     //Maximum elevator angle
#define PRESSURE_MIN 60  //Minimum shot pressure
#define PRESSURE_MAX 100 //Maximum shot pressure
#define DURATION_MIN 0   //Minimum valve duration
#define DURATION_MAX 10  //Maximum valve duration

FTDebouncer pinDebouncer(30);
//RotaryKnobController rotaryKnob(ENCODER_PIN_CLK, ENCODER_PIN_DT);
//Menu Setup
Printer dashPrinters[DASH_PRINTER_LEN]{{2, 0, "Dash", false}};
Printer elevatorPrinters[ELEVATOR_PRINTER_LEN]{{2, 0, "Elevator Angle", false}, {6, 1, "<[", true}, {8, 1, "!modifyValue!", false}, {10, 1, "]>", true}};
Printer shotPrinters[SHOT_PRINTER_LEN]{{2, 0, "Shot Pressure", false}, {6, 1, "<[", true}, {8, 1, "!modifyValue!", false}, {10, 1, "]>", true}};
Printer valvePrinters[VALVE_PRINTER_LEN]{{2, 0, "Valve Duration", false}, {6, 1, "<[", true}, {8, 1, "!modifyValue!", false}, {10, 1, "]>", true}};

DisplayPage displayPages[MENU_SIZE];

MenuController menuController(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH, MENU_SIZE);

//Joystick setup
JoystickController joystick(JOYSTICK_PIN_VRX, JOYSTICK_PIN_VRY, X_DEAD_ZONE_SIZE, Y_DEAD_ZONE_SIZE, X_MIN, X_MAX, Y_MIN, Y_MAX);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    pinDebouncer.addPin(ENCODER_PIN_SW, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();

    displayPages[0].displayPageInit(25, 0, 0, false, DASH_PRINTER_LEN, dashPrinters);

    displayPages[1].displayPageInit(1, ANGLE_MIN, ANGLE_MAX, true, ELEVATOR_PRINTER_LEN, elevatorPrinters);

    displayPages[2].displayPageInit(1, PRESSURE_MIN, PRESSURE_MAX, true, SHOT_PRINTER_LEN, shotPrinters);

    displayPages[3].displayPageInit(1, DURATION_MIN, DURATION_MAX, true, VALVE_PRINTER_LEN, valvePrinters);

    menuController.menuInit(displayPages);
}

void loop()
{
    menuController.menuUpdate();
    joystick.update();
    //rotaryKnob.getValue();
    pinDebouncer.update();
    Serial.println("X: " + String(joystick.getX()));
    Serial.println("Y: " + String(joystick.getY()));
}

//Methods for debouncer
void onPinActivated(int pinNr)
{
    switch (pinNr)
    {
    case ENCODER_PIN_SW:
        menuController.menuPress();
        break;
    }
}
void onPinDeactivated(int pinNr) {}