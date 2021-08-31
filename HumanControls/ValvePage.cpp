#include "ValvePage.h"

ValvePage::ValvePage(int increment, int min, int max) : Page(increment, min, max, true, Page::PageName::VALVE_PAGE)
{
}

void ValvePage::paint(DisplayController &display, bool isActivated, String status)
{
    display.clear();
    if (canActivate() && isActivated)
    {
        display.printRegion(5, 1, "<[");
        display.printRegion(10, 1, "]>");
    }
    display.printRegion(1, 0, "Valve Duration");
    display.printRegion(7, 1, String(getModifyValue()));
}