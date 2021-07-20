#include "FTDebouncer.h"
#include "RotaryKnobController.h"
#include "MenuController.h"
#include "CharacterDisplayNumberSix.h"

#define ENCODER_PIN_CLK 3    //CLK gets degrees for rotary knob
#define ENCODER_PIN_DT 4     //DT gets direction for rotary knob
#define ENCODER_PIN_SW 5     //Gets the button for rotary knob
#define DISPLAY_ADDRESS 0X27 //I2c address of the lcd display
#define DISPLAY_LENGTH 16    //Length of the lcd display
#define DISPLAY_WIDTH 2      //width of the lcd display

#define USB_BAUDRATE 115200

FTDebouncer pinDebouncer(30);
//RotaryKnobController rotaryKnob(ENCODER_PIN_CLK, ENCODER_PIN_DT);
MenuController menuController(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    pinDebouncer.addPin(ENCODER_PIN_SW, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();
    menuController.menuInit();
}

void loop()
{
    menuController.clockwisedate();
    pinDebouncer.update();
}

//Methods for debouncer
void onPinActivated(int pinNr)
{
    switch (pinNr)
    {
    case ENCODER_PIN_SW:
        menuController.menuSelect();
        break;
    }
}
void onPinDeactivated(int pinNr) {}