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
    display.printRegion(0, 1, "F:");

    display.printRegion(3, 1, String(object["fPr"].asInt()));
    display.printRegion(11, 1, "T:");
    display.printRegion(13, 1, String(object["tPr"].asInt()));

    if (isActivated)
    {
        display.printRegion(2, 1, this->m_downArrow);
        if (object["fPr"].asInt() >= 100)
        {
            display.printRegion(6, 1, this->m_upArrow);
        }
        else
        {
            display.printRegion(5, 1, this->m_upArrow);
        }
    }
}

void ShotPage::clockwise(JsonElement &object)
{
    JsonElement &frPres = object["fPr"];
    if (frPres.asInt() < (this->m_max - this->m_increment))
    {
        frPres = frPres.asInt() + this->m_increment;
    }
    else
    {
        frPres = this->m_max;
    }
}
void ShotPage::counterClockwise(JsonElement &object)
{
    JsonElement &frPres = object["fPr"];
    if (frPres.asInt() > (this->m_min + this->m_increment))
    {
        frPres = frPres.asInt() - this->m_increment;
    }
    else
    {
        frPres = this->m_min;
    }
}