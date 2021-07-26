#ifndef DISPLAYPAGE_H
#define DISPLAYPAGE_H
#include "Arduino.h"
#include "DisplayInterface.h"
#include "RotaryKnobController.h"
#include "DisplayController.h"

class DisplayPage : DisplayInterface
{
public:
    DisplayPage();
    DisplayPage(String name, bool isActivate);
    void displayPageInit(String name, bool isActivate);
    ~DisplayPage();
    void cleanUp(DisplayController &display);
    void paint(DisplayController &display, bool isActive);
    void clockwise();
    void counterClockwise();
    bool canActivate();

private:
    bool isActivate;
    String name;
    int value;
};
#endif