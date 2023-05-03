#ifndef DISPLAYCONTROLLER_H
#define DISPLAYCONTROLLER_H

#include <Wire.h>
#include <SerLCD.h>
#include <Arduino.h>
#include "CharacterDisplay.h"

class DisplayController : CharacterDisplay
{
public:
    // DisplayController(unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth);
    void init();
    void clear();
    void stringSetRegion(int x, int y, const char *text);
    void intSetRegion(int x, int y, int num);
    void print();
    // void printRegion(int x, int y, String text);
    // void printRegion(int x, int y, int code);
    // void printBattery(int x, int y, int code, double level);
    // byte isFilled(double level, double threshold);

private:
    SerLCD m_lcd;
    char m_lcdText[33];
    char m_strInt[8];
};
#endif
