#ifndef DISPLAYINTERFACE_H
#define DISPLAYINTERFACE_H

#include "DisplayController.h"
#include "Arduino.h"

class DisplayInterface
{
public:
    virtual void cleanUp(DisplayController &display) = 0;
    virtual void paint(DisplayController &display, bool isActivated, String status) = 0;
    virtual void clockwise() = 0;
    virtual void counterClockwise() = 0;
    virtual bool canActivate() = 0;
};

#endif
