#include <LiquidCrystal_I2C.h>
#include <JsonEl.h>
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
#define DISPLAY_ADDRESS 0X27      //I2c address of the lcd display
#define DISPLAY_LENGTH 16         //Length of the lcd display
#define DISPLAY_WIDTH 2           //width of the lcd display
#define NUM_BUTTONS 3             //Number of buttons to give the debouncer
#define HANG_TIMER_DURATION 10000 //Amount in milliseconds to stay on a page before going to dash
#define USB_BAUDRATE 115200
#define DOWN_ARROW 0              //Custom char code for down arrow
#define UP_ARROW 1                //Custom char code for up arrow
#define ROBOT_BATTERY_CHAR 2      //Custom char code for robot battery bar
#define CONTROLLER_BATTERY_CHAR 3 //Custom char code for controller battery bar

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

//Joystick deadzones and max
#define X_DEAD_ZONE_SIZE 100 //Total size of the X deadzone
#define JOYSTICK_MAX 1023    //Maximum joystick value that comes from the sensor
#define Y_DEAD_ZONE_SIZE 100 //Total size of the y deadzone

// Set up the JSON State for the robot
JsonElement humanControlFields[] = {
    Json::Int("id", -1),
    Json::String("conn", "Disconnected"),
};

JsonElement eStopFields[] = {
    Json::Boolean("btn", true),
};

JsonElement robotStateFields[] = {
    Json::String("ver", "1.0.0"),
    Json::Int("tck", 0),
    Json::Int("avgTck", 0),
    Json::Int("up", 0),
    Json::String("status", "Disabled"),
    Json::Object("hCtrl", humanControlFields),
    Json::Object("eStop", eStopFields),
    Json::Float("bat", 0.85),
    Json::Float("angle", 20.0),
    Json::Float("tnkPres", 0.0),
    Json::Float("frPres", 60.336),
    Json::Int("vlvTm", 100),
    Json::Float("dVel", 0.0),
    Json::Float("dRot", 0.0),
    Json::Boolean("fire", false),
    Json::Boolean("fill", false),
    Json::String("err", "", 32),
};
JsonElement controllerState = Json::Object(robotStateFields);

JsonState state(controllerState);

HumanControls humanControls(state, ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH,
                            ANGLE_INCREMENT, ANGLE_MIN, ANGLE_MAX, PRESSURE_INCREMENT, PRESSURE_MIN, PRESSURE_MAX,
                            DURATION_INCREMENT, DURATION_MIN, DURATION_MAX, HANG_TIMER_DURATION, DOWN_ARROW,
                            UP_ARROW, ROBOT_BATTERY_CHAR, CONTROLLER_BATTERY_CHAR, NUM_BUTTONS, ENCODER_PIN_SW,
                            ENABLE_PIN, FIRE_PIN, JOYSTICK_PIN_VRX, X_DEAD_ZONE_SIZE, JOYSTICK_MAX,
                            JOYSTICK_PIN_VRY, Y_DEAD_ZONE_SIZE);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    humanControls.init(DOWN_ARROW, UP_ARROW);
}

void loop()
{

    humanControls.update();
}

void onPinActivated(int pinNr)
{
    humanControls.onPinActivated(pinNr);
}
void onPinDeactivated(int pinNr)
{
    humanControls.onPinDeactivated(pinNr);
}