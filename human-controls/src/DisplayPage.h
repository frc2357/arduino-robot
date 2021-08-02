#ifndef DISPLAYPAGE_H
#define DISPLAYPAGE_H
#include "Arduino.h"
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"
#include "Printer.h"

//For array of printers:
//A printer with text value of "!modifyValue! indicates where the modifiable integer will be inserted"
class DisplayPage : DisplayInterface
{
public:
    DisplayPage();
    DisplayPage(int increment, bool isActivate, int printersLength, Printer printers[]);
    void displayPageInit(int increment, bool isActivate, int printersLength, Printer printers[]);
    ~DisplayPage();
    void cleanUp(DisplayController &display);
    void paint(DisplayController &display, bool isActive);
    void clockwise();
    void counterClockwise();
    bool canActivate();

private:
    bool isActivate;
    int modifyValue, increment, printersLength;
    Printer *printers;
};
#endif