#include "ElevatorPage.h"

ElevatorPage::ElevatorPage(int increment, int min, int max) : Page(increment, min, max, true, Page::PageType::ELEVATOR_PAGE)
{
}

void ElevatorPage::paint(DisplayController &display, bool isActivated, const char *status)
{
    display.clear();
    if (canActivate() && isActivated)
    {
        display.printRegion(5, 1, "<[");
        display.printRegion(9, 1, "]>");
    }
    display.printRegion(1, 0, "Elevator Angle");
    display.printRegion(7, 1, String(getModifyValue()));
}