#include "DashPage.h"

DashPage::DashPage() : Page(Page::PageType::DASH_PAGE)
{
}

void DashPage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();
    //display.printRegion(2, 0, "Dash");
    //display.printRegion(2, 1, state.root()["status"].asString());

    display.printRegion(0, 0, "S:");

    /* if (state.root()["status"].asString() == HumanControls::STATUS_DISABLED)
    {
        display.printRegion(0, 2, "D");
    }
    else if (state.root()["status"].asString() == HumanControls::STATUS_ENABLED)
    {
        display.printRegion(0, 2, "E");
    }
    else if (state.root()["status"].asString() == HumanControls::STATUS_PRIMED)
    {
        display.printRegion(0, 2, "P");
    }
    else
    {
        display.printRegion(0, 2, "!");
    }*/

    display.printRegion(10, 0, "Bat:");
    display.printRegion(14, 0, String(state.root()["bat"].asFloat() * 100));
}

void DashPage::clockwise(JsonState &state)
{
    return;
}
void DashPage::counterClockwise(JsonState &state)
{
    return;
}
