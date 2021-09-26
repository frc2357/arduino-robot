#include "DashPage.h"

DashPage::DashPage() : Page(Page::PageType::DASH_PAGE)
{
}

void DashPage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();

    Serial.println(state.root()["hCtrl"]["conn"].asString());
    if (state.root()["hCtrl"]["conn"].asString()[0] == 'A')
    {
        display.printRegion(0, 0, state.root()["status"].asString());
    }
    else
    {
        display.printRegion(0, 0, state.root()["hCtrl"]["conn"].asString());
    }

    display.printRegion(12, 0, "R");
    display.printBattery(13, 0, 2, state.root()["bat"].asFloat() * 100);
    display.printRegion(14, 0, "C");
    display.printBattery(15, 0, 3, 30.0);

    display.printRegion(0, 1, "A:");
    display.printRegion(2, 1, String(static_cast<int>(state.root()["angle"].asFloat())));

    display.printRegion(5, 1, "P:");
    display.printRegion(7, 1, String(static_cast<int>(state.root()["frPres"].asFloat())));

    display.printRegion(11, 1, "D:");
    display.printRegion(13, 1, String(state.root()["vlvTm"].asInt()));
}

void DashPage::clockwise(JsonState &state)
{
    return;
}
void DashPage::counterClockwise(JsonState &state)
{
    return;
}
