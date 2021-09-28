#ifndef DISPLAYINTERFACE_H
#define DISPLAYINTERFACE_H

#include "DisplayController.h"
#include "Arduino.h"
#include <JsonEl.h>

class DisplayInterface
{
public:
    virtual void cleanUp(DisplayController &display) = 0;
    virtual void paint(DisplayController &display, bool isActivated, JsonElement &object) = 0;
    virtual void clockwise(JsonElement &object) = 0;
    virtual void counterClockwise(JsonElement &object) = 0;
    virtual bool canActivate() = 0;
    virtual bool applyHang() = 0;
};

#endif
