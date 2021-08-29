#include "DashPage.h"

DashPage::DashPage() : Page()
{
}

void DashPage::paint(DisplayController &display, bool isActivated, String status)
{
    display.clear();
    display.printRegion(2, 0, "Dash");
    display.printRegion(2, 1, status);
}
