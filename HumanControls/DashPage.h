#ifndef DASH_PAGE_H
#define DASH_PAGE_h

#include "Page.h"
#include "Arduino.h"
//#include "HumanControls.h"
#include <JsonEl.h>

class DashPage : public Page
{
public:
    DashPage();
    void paint(DisplayController &display, bool isActivated, JsonState &state);
    void clockwise(JsonState &state);
    void counterClockwise(JsonState &state);
};

#endif