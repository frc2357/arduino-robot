#ifndef VALVE_PAGE_H
#define VALVE_PAGE_h

#include "Page.h"
#include "Arduino.h"

class ValvePage : public Page
{
public:
    ValvePage(int increment, int min, int max);
    void paint(DisplayController &display, bool isActivated, const char *status);
};

#endif