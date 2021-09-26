#include "ElevatorPage.h"

ElevatorPage::ElevatorPage(int increment, int min, int max) : Page(true, Page::PageType::ELEVATOR_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ElevatorPage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();

    display.printRegion(1, 0, "Elevator Angle");
    display.printRegion(5, 1, String(state.root()["angle"].asFloat()));

    if (canActivate() && isActivated)
    {
        display.printRegion(4, 1, 0);
        display.printRegion(10, 1, 1);
    }
}

void ElevatorPage::clockwise(JsonState &state)
{
    if (state.root()["angle"].asFloat() < this->m_max)
    {
        state.root()["angle"] = state.root()["angle"].asFloat() + this->m_increment;
    }
    else
    {
        state.root()["angle"] = this->m_max;
    }
}
void ElevatorPage::counterClockwise(JsonState &state)
{
    if (state.root()["angle"].asFloat() > this->m_min)
    {
        state.root()["angle"] = state.root()["angle"].asFloat() - this->m_increment;
    }
    else
    {
        state.root()["angle"] = this->m_min;
    }
}