#ifndef DASH_PAGE_H
#define DASH_PAGE_h

#include "Page.h"
#include <Arduino.h>
#include <JsonElement.h>
#include "TShirtCannonPayload.h"

class DashPage : public Page
{
public:
    DashPage(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar, unsigned int controllerBatChar);
    void paint(DisplayController &display, bool isActivated, JsonElement &object);
    void clockwise(JsonElement &object);
    void counterClockwise(JsonElement &object);
    int rangeFilter(int value);
};

#endif