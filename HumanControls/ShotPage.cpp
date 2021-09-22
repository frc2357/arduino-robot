#include "ShotPage.h"

ShotPage::ShotPage(int increment, int min, int max) : Page(true, Page::PageType::SHOT_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ShotPage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();
    if (canActivate() && isActivated)
    {
        display.printRegion(2, 1, "<[");
        display.printRegion(7, 1, "]>");
    }

    display.printRegion(1, 0, "Shot Pressure");
    display.printRegion(9, 1, "T:");
    display.printRegion(11, 1, String(state.root()["tnkPres"].asFloat()));
    display.printRegion(4, 1, String(state.root()["frPres"].asFloat()));
}

void ShotPage::clockwise(JsonState &state)
{
    if (state.root()["frPres"].asFloat() < this->m_max)
    {
        state.root()["frPres"] = state.root()["frPres"].asFloat() + this->m_increment;
    }
}
void ShotPage::counterClockwise(JsonState &state)
{
    if (state.root()["frPres"].asFloat() > this->m_min)
    {
        state.root()["frPres"] = state.root()["frPres"].asFloat() - this->m_increment;
    }
}