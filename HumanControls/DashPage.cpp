#include "DashPage.h"

DashPage::DashPage() : Page(Page::PageType::DASH_PAGE)
{
}

void DashPage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();
    display.printRegion(2, 0, "Dash");
    display.printRegion(2, 1, state.root()["status"].asString());
}
