#ifndef DEBUG_PAGE_H
#define DEBUG_PAGE_h

#include "Page.h"
#include <Arduino.h>
#include <JsonElement.h>

class DebugPage : public Page
{
public:
    DebugPage(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar, unsigned int controllerBatChar);
    void paint(DisplayController &display, bool isActivated, JsonElement &object);
    void clockwise(JsonElement &object);
    void counterClockwise(JsonElement &object);
};

#endif