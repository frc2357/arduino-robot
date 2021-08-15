#ifndef CHARACTERDISPLAY_H
#define CHARACTERDISPLAY_H

#include "Arduino.h"

class CharacterDisplay
{
public:
    virtual void clear() = 0;
    virtual void printRegion(int x, int y, String text) = 0;
};
#endif
