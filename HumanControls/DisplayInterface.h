#ifndef DISPLAYINTERFACE_H
#define DISPLAYINTERFACE_H

#include "DisplayController.h"
#include "Arduino.h"
#include <JsonEl.h>

class DisplayInterface
{
public:
    virtual void cleanUp(DisplayController &display) = 0;
    virtual void paint(DisplayController &display, bool isActivated, JsonState &state) = 0;
    virtual void clockwise(JsonState &state) = 0;
    virtual void counterClockwise(JsonState &state) = 0;
    virtual bool canActivate() = 0;
};

#endif
