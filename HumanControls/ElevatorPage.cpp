#include "ElevatorPage.h"

ElevatorPage::ElevatorPage(int increment, int min, int max, unsigned int downArrow,
                           unsigned int upArrow, unsigned int robotBatChar, unsigned int controllerBatChar)
    : Page(true, true, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::ELEVATOR_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ElevatorPage::paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload)
{
    display.clear();

    display.printRegion(1, 0, "Elevator Angle");
    display.printRegion(7, 1, String(payload.m_angle.asInt()));

    if (isActivated)
    {
        display.printRegion(6, 1, this->m_upArrow);
        display.printRegion(9, 1, this->m_downArrow);
    }
}

void ElevatorPage::clockwise(TShirtCannonPayload &payload)
{
    uint8_t &angle = payload.m_angle;
    angle = angle.asInt() + this->m_increment;
    angle = rangeFilter(angle.asInt());
}
void ElevatorPage::counterClockwise(TShirtCannonPayload &payload)
{
    uint8_t &angle = payload.m_angle;
    angle = angle.asInt() - this->m_increment;
    angle = rangeFilter(angle.asInt());
}

int ElevatorPage::rangeFilter(int value)
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