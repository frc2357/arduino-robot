#ifndef ELEVATOR_PAGE_H
#define ELEVATOR_PAGE_h

#include "Page.h"
#include <Arduino.h>
#include <JsonElement.h>
#include "TShirtCannonPayload.h"

class ElevatorPage : public Page
{
public:
    ElevatorPage(int increment, int min, int max, unsigned int downArrow, unsigned int upArrow,
                 unsigned int robotBatChar, unsigned int controllerBatChar);
    void paint(DisplayController &display, bool isActivated, JsonElement &object);
    void clockwise(JsonElement &object);
    void counterClockwise(JsonElement &object);
    int rangeFilter(int value);

private:
    int m_min, m_max, m_increment;
};

#endif