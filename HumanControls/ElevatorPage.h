#ifndef ELEVATOR_PAGE_H
#define ELEVATOR_PAGE_h

#include "Page.h"
#include "Arduino.h"

class ElevatorPage : public Page
{
public:
    ElevatorPage(int increment, int min, int max);
    void paint(DisplayController &display, bool isActivated, String Status);
};

#endif