#ifndef CHARACTERDISPLAY_H
#define CHARACTERDISPLAY_H

#include <Arduino.h>

class CharacterDisplay
{
public:
    virtual void clear() = 0;
    // virtual void printRegion(int x, int y, String text) = 0;
    // virtual void printRegion(int x, int y, int code) = 0;
    // virtual void printBattery(int x, int y, int code, double level) = 0;
    // virtual byte isFilled(double level, double threshold) = 0;
};
#endif
