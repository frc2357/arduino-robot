#ifndef VALVE_PAGE_H
#define VALVE_PAGE_h

#include "Page.h"
#include "Arduino.h"
#include <JsonEl.h>

class ValvePage : public Page
{
public:
    ValvePage(int increment, int min, int max, unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar, unsigned int controllerBatChar);
    void paint(DisplayController &display, bool isActivated, JsonState &state);
    void clockwise(JsonState &state);
    void counterClockwise(JsonState &state);

private:
    int m_min, m_max, m_increment;
};

#endif