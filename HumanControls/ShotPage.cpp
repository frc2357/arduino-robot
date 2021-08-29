#include "ShotPage.h"

ShotPage::ShotPage(int increment, int min, int max) : Page(increment, min, max, true)
{
}

void ShotPage::paint(DisplayController &display, bool isActivated, String status)
{
    display.clear();
    if (canActivate() && isActivated)
    {
        display.printRegion(5, 1, "<[");
        display.printRegion(10, 1, "]>");
    }
    display.printRegion(1, 0, "Shot Pressure");
    display.printRegion(7, 1, String(getModifyValue()));
}