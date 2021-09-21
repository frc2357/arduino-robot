#include "ShotPage.h"

ShotPage::ShotPage(int increment, int min, int max) : Page(increment, min, max, true, Page::PageType::SHOT_PAGE)
{
}

void ShotPage::paint(DisplayController &display, bool isActivated, const char *status)
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