#include "ValvePage.h"

ValvePage::ValvePage(int increment, int min, int max) : Page(true, Page::PageType::VALVE_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ValvePage::paint(DisplayController &display, bool isActivated, JsonState &state)
{
    display.clear();
    if (canActivate() && isActivated)
    {
        display.printRegion(5, 1, "<[");
        display.printRegion(10, 1, "]>");
    }
    display.printRegion(1, 0, "Valve Duration");
    display.printRegion(7, 1, String(state.root()["vlvTm"].asInt()));
}

void ValvePage::clockwise(JsonState &state)
{
    if (state.root()["vlvTm"].asInt() < this->m_max)
    {
        state.root()["vlvTm"] = state.root()["vlvTm"].asInt() + this->m_increment;
    }
}
void ValvePage::counterClockwise(JsonState &state)
{
    if (state.root()["vlvTm"].asInt() > this->m_min)
    {
        state.root()["vlvTm"] = state.root()["vlvTm"].asInt() + this->m_increment;
    }
}