#include "ValvePage.h"

ValvePage::ValvePage(int increment, int min, int max, unsigned int downArrow, unsigned int upArrow,
                     unsigned int robotBatChar, unsigned int controllerBatChar)
    : Page(true, true, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::VALVE_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ValvePage::paint(DisplayController &display, bool isActivated, JsonElement &object)
{
    display.clear();

    display.printRegion(1, 0, "Valve Duration");
    display.printRegion(6, 1, String(object["vlv"].asInt()));

    if (isActivated)
    {
        display.printRegion(5, 1, this->m_downArrow);
        display.printRegion(9, 1, this->m_upArrow);
    }
}

void ValvePage::clockwise(JsonElement &object)
{
    JsonElement &vlvTm = object["vlv"];
    vlvTm = vlvTm.asInt() + this->m_increment;
    vlvTm = rangeFilter(vlvTm.asInt());
}
void ValvePage::counterClockwise(JsonElement &object)
{
    JsonElement &vlvTm = object["vlv"];
    vlvTm = vlvTm.asInt() - this->m_increment;
    vlvTm = rangeFilter(vlvTm.asInt());
}

int ValvePage::rangeFilter(int value)
{
    if (value < this->m_min)
    {
        return this->m_min;
    }
    else if (value > this->m_max)
    {
        return this->m_max;
    }
    return value;
}