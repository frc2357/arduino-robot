#include "ShotPage.h"

ShotPage::ShotPage(int increment, int min, int max, unsigned int downArrow, unsigned int upArrow,
                   unsigned int robotBatChar, unsigned int controllerBatChar)
    : Page(true, true, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::SHOT_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ShotPage::paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload)
{
    display.clear();

    display.printRegion(1, 0, "Shot Pressure");
    display.printRegion(0, 1, "F:");

    display.printRegion(3, 1, String(payload.m_firingPressure.asInt()));
    display.printRegion(11, 1, "T:");
    display.printRegion(13, 1, String(payload.m_tankPressure.asInt()));

    if (isActivated)
    {
        display.printRegion(2, 1, this->m_downArrow);
        if (payload.m_firingPressure.asInt() >= 100)
        {
            display.printRegion(6, 1, this->m_upArrow);
        }
        else
        {
            display.printRegion(5, 1, this->m_upArrow);
        }
    }
}

void ShotPage::clockwise(TShirtCannonPayload &payload)
{
    JsonElement &frPres = payload.m_firingPressure;
    frPres = frPres.asInt() + this->m_increment;
    frPres = rangeFilter(frPres.asInt());
}
void ShotPage::counterClockwise(TShirtCannonPayload &payload)
{
    JsonElement &frPres = payload.m_firingPressure;
    frPres = frPres.asInt() - this->m_increment;
    frPres = rangeFilter(frPres.asInt());
}

int ShotPage::rangeFilter(int value)
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