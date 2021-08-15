#include "CharacterDisplay.h"
//#include "FTDebouncer.h"
//#include "RotaryKnobController.h"
//#include "CharacterDisplay.h"
//#include "MenuController.h"
//#include "DisplayController.h"
//#include "Page.h"
//#include "Printer.h"
//#include "CharacterDisplay.h"

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

//Lengths of printers inside each Page
#define DASH_PRINTER_LEN 1
#define ELEVATOR_PRINTER_LEN 4
#define SHOT_PRINTER_LEN 4
#define VALVE_PRINTER_LEN 4

//Min - Max
#define ANGLE_MIN 0      //Minimum elevator angle
#define ANGLE_MAX 10     //Maximum elevator angle
#define PRESSURE_MIN 60  //Minimum shot pressure
#define PRESSURE_MAX 100 //Maximum shot pressure
#define DURATION_MIN 0   //Minimum valve duration
#define DURATION_MAX 10  //Maximum valve duration

FTDebouncer pinDebouncer(30);
//RotaryKnobController rotaryKnob(ENCODER_PIN_CLK, ENCODER_PIN_DT);

Printer dashPrinters[DASH_PRINTER_LEN]{{2, 0, "Dash", false}};
Printer elevatorPrinters[ELEVATOR_PRINTER_LEN]{{2, 0, "Elevator Angle", false}, {6, 1, "<[", true}, {8, 1, "!modifyValue!", false}, {10, 1, "]>", true}};
Printer shotPrinters[SHOT_PRINTER_LEN]{{2, 0, "Shot Pressure", false}, {6, 1, "<[", true}, {8, 1, "!modifyValue!", false}, {10, 1, "]>", true}};
Printer valvePrinters[VALVE_PRINTER_LEN]{{2, 0, "Valve Duration", false}, {6, 1, "<[", true}, {8, 1, "!modifyValue!", false}, {10, 1, "]>", true}};

Page pages[MENU_SIZE];

MenuController menuController(ENCODER_PIN_CLK, ENCODER_PIN_DT, DISPLAY_ADDRESS, DISPLAY_LENGTH, DISPLAY_WIDTH, MENU_SIZE);

void setup()
{
    Serial.begin(USB_BAUDRATE);
    pinDebouncer.addPin(ENCODER_PIN_SW, HIGH, INPUT_PULLUP);
    pinDebouncer.begin();

    pages[0].init(25, 0, 0, false, DASH_PRINTER_LEN, dashPrinters);

    pages[1].init(1, ANGLE_MIN, ANGLE_MAX, true, ELEVATOR_PRINTER_LEN, elevatorPrinters);

    pages[2].init(1, PRESSURE_MIN, PRESSURE_MAX, true, SHOT_PRINTER_LEN, shotPrinters);

    pages[3].init(1, DURATION_MIN, DURATION_MAX, true, VALVE_PRINTER_LEN, valvePrinters);

    menuController.init(pages);
}

void loop()
{
    menuController.menuUpdate();
    //rotaryKnob.getValue();
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
