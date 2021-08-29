#ifndef DASH_PAGE_H
#define DASH_PAGE_h

#include "Page.h"
#include "Arduino.h"

class DashPage : public Page
{
public:
    DashPage();
    void paint(DisplayController &display, bool isActivated, String status);
};

#endif