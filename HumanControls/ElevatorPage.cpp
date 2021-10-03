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
    display.printRegion(5, 1, String(object["angle"].asFloat()));

    if (isActivated)
    {
        display.printRegion(4, 1, this->m_upArrow);
        display.printRegion(10, 1, this->m_downArrow);
    }
}

void ElevatorPage::clockwise(JsonElement &object)
{
    JsonElement &angle = object["angle"];
    if (angle.asFloat() < (this->m_max - this->m_increment))
    {
        angle = angle.asFloat() + this->m_increment;
    }
    else
    {
        angle = this->m_max;
    }
}
void ElevatorPage::counterClockwise(JsonElement &object)
{
    JsonElement &angle = object["angle"];
    if (angle.asFloat() > (this->m_min + this->m_increment))
    {
        angle = angle.asFloat() - this->m_increment;
    }
    else
    {
        angle = this->m_min;
    }
}