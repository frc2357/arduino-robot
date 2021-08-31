#include "LiquidCrystal.h"
#include <LiquidCrystal_I2C.h>
#include "FTDebouncer.h"
#include "RotaryKnobController.h"
#include "CharacterDisplay.h"
#include "MenuController.h"
#include "DisplayController.h"
#include "Page.h"
#include "CharacterDisplay.h"
#include "EnableController.h"
#include "FireController.h"

//Pins
#define ENCODER_PIN_CLK 3 //CLK gets degrees for rotary knob
#define ENCODER_PIN_DT 4  //DT gets direction for rotary knob
#define ENCODER_PIN_SW 5  //Gets the button for rotary knob
#define ENABLE_PIN 7      //Digital Pin for the enable button
#define FIRE_PIN 9        //Digital Pin for the fire button

//Other constraints
#define DISPLAY_ADDRESS 0X27 //I2c address of the lcd display
#define DISPLAY_LENGTH 16    //Length of the lcd display
#define DISPLAY_WIDTH 2      //width of the lcd display
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

//Make array of states, diabled, enabled, primed
//Menu button sets primed state
//Make isConnected boolean

FTDebouncer pinDebouncer(30);

MenuController menuController(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH,
                              ANGLE_INCREMENT, ANGLE_MIN, ANGLE_MAX, PRESSURE_INCREMENT, PRESSURE_MIN, PRESSURE_MAX,
                              DURATION_INCREMENT, DURATION_MIN, DURATION_MAX);

EnableController enableController;
FireController fireController;

bool isConnected = false;
String statuses[3] = {"disabled", "enabled", "primed"};
String status = statuses[0];
String lastStatus = "";

void setup()
{
    Serial.begin(USB_BAUDRATE);
    connect();
    pinDebouncer.addPin(ENCODER_PIN_SW, HIGH, INPUT_PULLUP);
    pinDebouncer.addPin(ENABLE_PIN, HIGH, INPUT_PULLUP);
    pinDebouncer.addPin(FIRE_PIN, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();
    menuController.init(status);
}

void loop()
{
    setStatus();

    menuController.menuUpdate(status);
    pinDebouncer.update();
}

//Methods for debouncer
void onPinActivated(int pinNr)
{
    switch (pinNr)
    {
    case ENCODER_PIN_SW:
        menuController.menuPress(status, statuses, fireController);
        break;
    case ENABLE_PIN:
        enableController.setIsEnabled(true);
        break;
    case FIRE_PIN:
        //Eventually will use to tell Robot to fire through JSON
        if (status == statuses[2])
        {
            fireController.initiateFiring(true);
        }
        break;
    }
}

void onPinDeactivated(int pinNr)
{
    switch (pinNr)
    {
    case ENABLE_PIN:
        enableController.setIsEnabled(false);
        fireController.setIsFireToggled(false);
        break;
    }
}

void setStatus()
{
    if (isConnected)
    {
        if (enableController.getIsEnabled())
        {
            if (fireController.getIsFireToggled())
            {
                status = statuses[2];
            }
            else
            {
                status = statuses[1];
            }
        }
        else
        {
            status = statuses[0];
        }
    }
    else
    {
        status = "disconeccted";
    }

    if (status != lastStatus)
    {
        Serial.println(status);
        lastStatus = status;
        menuController.menuRefresh(status);
    }
}

void connect()
{
    //Connect to the robot
    isConnected = true;
}