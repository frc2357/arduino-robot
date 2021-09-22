#ifndef ELEVATOR_PAGE_H
#define ELEVATOR_PAGE_h

#include "Page.h"
#include "Arduino.h"
#include <JsonEl.h>

class ElevatorPage : public Page
{
public:
    ElevatorPage(int increment, int min, int max);
    void paint(DisplayController &display, bool isActivated, JsonState &state);
    void clockwise(JsonState &state);
    void counterClockwise(JsonState &state);

private:
    int m_min, m_max, m_increment;
};

#endif