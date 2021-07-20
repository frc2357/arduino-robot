#ifndef CHARACTERDISPLAYNUMBERSIX_H
#define CHARACTERDISPLAYNUMBERSIX_H

#include "Arduino.h"

class CharacterDisplayNumberSix
{
public:
    virtual void clear() = 0;
    virtual void printRegion(int x, int y, String text) = 0;
};
#endif