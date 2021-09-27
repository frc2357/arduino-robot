#include "ShotPage.h"

ShotPage::ShotPage(int increment, int min, int max, unsigned int downArrow, unsigned int upArrow,
                   unsigned int robotBatChar, unsigned int controllerBatChar)
    : Page(true, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::SHOT_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ShotPage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();

    display.printRegion(1, 0, "Shot Pressure");
    display.printRegion(8, 1, "C:");
    display.printRegion(10, 1, String(state.root()["tnkPres"].asFloat()));
    display.printRegion(1, 1, String(state.root()["frPres"].asFloat()));

    if (canActivate() && isActivated)
    {
        display.printRegion(0, 1, this->m_downArrow);
        if (state.root()["frPres"].asFloat() >= 100)
        {
            display.printRegion(7, 1, this->m_upArrow);
        }
        else
        {
            display.printRegion(6, 1, this->m_upArrow);
        }
    }
}

void ShotPage::clockwise(JsonState &state)
{
    if (state.root()["frPres"].asFloat() < this->m_max)
    {
        state.root()["frPres"] = state.root()["frPres"].asFloat() + this->m_increment;
    }
    else
    {
        state.root()["frPres"] = this->m_max;
    }
}
void ShotPage::counterClockwise(JsonState &state)
{
    if (state.root()["frPres"].asFloat() > this->m_min)
    {
        state.root()["frPres"] = state.root()["frPres"].asFloat() - this->m_increment;
    }
    else
    {
        state.root()["frPres"] = this->m_min;
    }
}