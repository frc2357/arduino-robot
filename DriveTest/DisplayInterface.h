#ifndef DISPLAYINTERFACE_H
#define DISPLAYINTERFACE_H

#include "DisplayController.h"
#include <Arduino.h>
#include <JsonElement.h>
#include "TShirtCannonPayload.h"

class DisplayInterface
{
public:
    virtual void cleanUp(DisplayController &display) = 0;
    virtual void paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload) = 0;
    virtual void clockwise(TShirtCannonPayload &payload) = 0;
    virtual void counterClockwise(TShirtCannonPayload &payload) = 0;
    virtual int rangeFilter(int value) = 0;
    virtual bool canActivate() = 0;
    virtual bool applyHang() = 0;
};

#endif
