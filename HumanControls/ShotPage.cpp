#include "ShotPage.h"

ShotPage::ShotPage(int increment, int min, int max, unsigned int downArrow, unsigned int upArrow,
                   unsigned int robotBatChar, unsigned int controllerBatChar)
    : Page(true, true, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::SHOT_PAGE)
{
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
}

void ShotPage::paint(DisplayController &display, bool isActivated, JsonElement &object)
{
    display.clear();

    display.printRegion(1, 0, "Shot Pressure");
    display.printRegion(8, 1, "C:");
    display.printRegion(10, 1, String(object["tnkPres"].asFloat()));
    display.printRegion(1, 1, String(object["frPres"].asFloat()));

    if (canActivate() && isActivated)
    {
        display.printRegion(0, 1, this->m_downArrow);
        if (object["frPres"].asFloat() >= 100)
        {
            display.printRegion(7, 1, this->m_upArrow);
        }
        else
        {
            display.printRegion(6, 1, this->m_upArrow);
        }
    }
}

void ShotPage::clockwise(JsonElement &object)
{
    JsonElement &frPres = object["frPres"];
    if (frPres.asFloat() < (this->m_max - this->m_increment))
    {
        frPres = frPres.asFloat() + this->m_increment;
    }
    else
    {
        frPres = this->m_max;
    }
}
void ShotPage::counterClockwise(JsonElement &object)
{
    JsonElement &frPres = object["frPres"];
    if (frPres.asFloat() > (this->m_min + this->m_increment))
    {
        frPres = frPres.asFloat() - this->m_increment;
    }
    else
    {
        frPres = this->m_min;
    }
}