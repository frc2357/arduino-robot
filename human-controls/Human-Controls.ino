#include "FTDebouncer.h"
#include "RotaryKnobController.h"

#define ENCODER_PIN_CLK 3 //CLK gets degrees for rotary knob
#define ENCODER_PIN_DT 4  //DT gets direction for rotary knob
#define ENCODER_PIN_SW 5  //Gets the button for rotary knob

#define USB_BAUDRATE 115200

FTDebouncer pinDebouncer(30);
RotaryKnobController rotaryKnob(ENCODER_PIN_CLK, ENCODER_PIN_DT);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    pinDebouncer.addPin(ENCODER_PIN_SW, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();
}

void loop()
{
    rotaryKnob.updateRotaryKnob();
    pinDebouncer.update();
}

//Methods for debouncer
void onPinActivated(int pinNr)
{
    switch (pinNr)
    {
    case ENCODER_PIN_SW:
        rotaryKnob.menuSelect();
        break;
    }
}
void onPinDeactivated(int pinNr) {}