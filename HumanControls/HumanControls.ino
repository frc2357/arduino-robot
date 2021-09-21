#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
#include "FTDebouncer.h"
#include "RotaryKnobController.h"
#include "CharacterDisplay.h"
#include "MenuController.h"
#include "DisplayController.h"
#include "Page.h"
#include "CharacterDisplay.h"
#include "JoystickAxis.h"
#include "EnableController.h"
#include "FireController.h"
#include "HumanControls.h"

//Pins
//LCD connects SDA in analog pin 4 and SCL in analog pin 5
#define ENCODER_PIN_CLK 3  //CLK gets degrees for rotary knob
#define ENCODER_PIN_DT 4   //DT gets direction for rotary knob
#define ENCODER_PIN_SW 5   //Gets the button for rotary knob
#define JOYSTICK_PIN_SW 6  //Button for joystick
#define JOYSTICK_PIN_VRX 0 //Analog Pin for joystick x
#define JOYSTICK_PIN_VRY 1 //Analog Pin for joystick y
#define ENABLE_PIN 7       //Digital Pin for the enable button
#define FIRE_PIN 9         //Digital Pin for the fire button

//Other constraints
#define DISPLAY_ADDRESS 0X27 //I2c address of the lcd display
#define DISPLAY_LENGTH 16    //Length of the lcd display
#define DISPLAY_WIDTH 2      //width of the lcd display
#define NUM_BUTTONS 3        //Number of buttons to give the debouncer
#define USB_BAUDRATE 115200

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

HumanControls humanControls(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH,
                            ANGLE_INCREMENT, ANGLE_MIN, ANGLE_MAX, PRESSURE_INCREMENT, PRESSURE_MIN, PRESSURE_MAX,
                            DURATION_INCREMENT, DURATION_MIN, DURATION_MAX, NUM_BUTTONS, ENCODER_PIN_SW,
                            ENABLE_PIN, FIRE_PIN);

//Joystick setup
JoystickAxis xAxis(JOYSTICK_PIN_VRX, X_DEAD_ZONE_SIZE, JOYSTICK_MAX);
JoystickAxis yAxis(JOYSTICK_PIN_VRY, Y_DEAD_ZONE_SIZE, JOYSTICK_MAX);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    humanControls.init();
}

void loop()
{

    humanControls.update();
    xAxis.update();
    yAxis.update();
    Serial.println("X Axis: " + String(xAxis.getResult()));
    //Serial.println("Y Axis: " + String(yAxis.getResult()));
}

void onPinActivated(int pinNr)
{
    humanControls.onPinActivated(pinNr);
}
void onPinDeactivated(int pinNr)
{
    humanControls.onPinDeactivated(pinNr);
}