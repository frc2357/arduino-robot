#include "LiquidCrystal.h"
#include <LiquidCrystal_I2C.h>
#include "FTDebouncer.h"
#include "RotaryKnobController.h"
#include "CharacterDisplay.h"
#include "MenuController.h"
#include "DisplayController.h"
#include "Page.h"
#include "CharacterDisplay.h"
#include "JoyStickController.h"

//Pins
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
#define USB_BAUDRATE 115200

//Joystick deadzones
#define X_DEAD_ZONE_SIZE 100 //Total size of the x deadzone
#define X_MIN 0
#define X_MAX 1023
#define Y_DEAD_ZONE_SIZE 100 //Total size of the y deadzone
#define Y_MIN 0
#define Y_MAX 1023

//Min - Max
#define ANGLE_INCREMENT 1     //Increment amount for elevator angle
#define ANGLE_MIN 20          //Minimum elevator angle
#define ANGLE_MAX 70          //Maximum elevator angle
#define PRESSURE_INCREMENT 1  //Increment amount for shot pressure
#define PRESSURE_MIN 60       //Minimum shot pressure
#define PRESSURE_MAX 120      //Maximum shot pressure
#define DURATION_INCREMENT 10 //Increment amount for valve duration
#define DURATION_MIN 100      //Minimum valve duration
#define DURATION_MAX 300      //Maximum valve duration

FTDebouncer pinDebouncer(30);

MenuController menuController(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH,
                              ANGLE_INCREMENT, ANGLE_MIN, ANGLE_MAX, PRESSURE_INCREMENT, PRESSURE_MIN, PRESSURE_MAX,
                              DURATION_INCREMENT, DURATION_MIN, DURATION_MAX);

//Joystick setup
JoystickController joystick(JOYSTICK_PIN_VRX, JOYSTICK_PIN_VRY, X_DEAD_ZONE_SIZE, Y_DEAD_ZONE_SIZE, X_MIN, X_MAX, Y_MIN, Y_MAX);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    pinDebouncer.addPin(ENCODER_PIN_SW, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();
    menuController.init();
}

void loop()
{
    menuController.menuUpdate();
    joystick.update();
    pinDebouncer.update();
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