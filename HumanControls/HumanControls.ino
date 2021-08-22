#include "LiquidCrystal.h"
#include <LiquidCrystal_I2C.h>
#include "FTDebouncer.h"
#include "RotaryKnobController.h"
#include "CharacterDisplay.h"
#include "MenuController.h"
#include "DisplayController.h"
#include "Page.h"
#include "CharacterDisplay.h"

//Pins
#define ENCODER_PIN_CLK 3 //CLK gets degrees for rotary knob
#define ENCODER_PIN_DT 4  //DT gets direction for rotary knob
#define ENCODER_PIN_SW 5  //Gets the button for rotary knob

//Other constraints
#define DISPLAY_ADDRESS 0X27 //I2c address of the lcd display
#define DISPLAY_LENGTH 16    //Length of the lcd display
#define DISPLAY_WIDTH 2      //width of the lcd display
#define MENU_SIZE 4          //Number of menu pages
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

FTDebouncer pinDebouncer(30);

Page pages[MENU_SIZE];

MenuController menuController(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH, MENU_SIZE,
                              ANGLE_INCREMENT, ANGLE_MIN, ANGLE_MAX, PRESSURE_INCREMENT, PRESSURE_MIN, PRESSURE_MAX,
                              DURATION_INCREMENT, DURATION_MIN, DURATION_MAX);

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