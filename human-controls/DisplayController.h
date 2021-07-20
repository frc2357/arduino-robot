#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Arduino.h"
//#include "CharacterDisplay.h"

class DisplayController : CharacterDisplay
{
public:
    DisplayController(unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth);
    ~DisplayController();
    void displayUpdate();
    void displayInit();
    void clockwise();
    void counterClockwise();
    void menuSelect();
    void clear();
    void printRegion(int x, int y, String text);

private:
    LiquidCrystal_I2C *lcd;
};
#endif
