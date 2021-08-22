#include "DashPage.h"

DashPage::DashPage() : Page()
{
}

void DashPage::paint(DisplayController &display, bool isActivated)
{
    display.clear();
    display.printRegion(2, 0, "Dash");
}
