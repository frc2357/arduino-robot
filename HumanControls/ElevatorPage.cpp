#include "ElevatorPage.h"

ElevatorPage::ElevatorPage(int increment, int min, int max, unsigned int downArrow,
                           unsigned int upArrow, unsigned int robotBatChar, unsigned int controllerBatChar)
    : Page(true, true, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::ELEVATOR_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ElevatorPage::paint(DisplayController &display, bool isActivated, JsonElement &object)
{
    display.clear();

    display.printRegion(1, 0, "Elevator Angle");
    display.printRegion(7, 1, String(object["ang"].asInt()));

    if (isActivated)
    {
        display.printRegion(6, 1, this->m_upArrow);
        display.printRegion(9, 1, this->m_downArrow);
    }
}

void ElevatorPage::clockwise(JsonElement &object)
{
    JsonElement &angle = object["ang"];
    if (angle.asInt() < (this->m_max - this->m_increment))
    {
        angle = angle.asInt() + this->m_increment;
    }
    else
    {
        angle = this->m_max;
    }
}
void ElevatorPage::counterClockwise(JsonElement &object)
{
    JsonElement &angle = object["ang"];
    if (angle.asInt() > (this->m_min + this->m_increment))
    {
        angle = angle.asInt() - this->m_increment;
    }
    else
    {
        angle = this->m_min;
    }
}