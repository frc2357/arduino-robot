#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Arduino.h>
#include "CharacterDisplay.h"

class DisplayController : CharacterDisplay
{
public:
    DisplayController(unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth);
    void init(unsigned int downArrow, unsigned int upArrow);
    void clear();
    void printRegion(int x, int y, String text);
    void printRegion(int x, int y, int code);
    void printBattery(int x, int y, int code, double level);
    byte isFilled(double level, double threshold);

private:
    LiquidCrystal_I2C m_lcd;
};
#endif
