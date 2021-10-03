#include "DashPage.h"

DashPage::DashPage(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
                   unsigned int controllerBatChar) : Page(true, false, downArrow, upArrow, robotBatChar, controllerBatChar,
                                                          Page::PageType::DASH_PAGE)
{
}

void DashPage::paint(DisplayController &display, bool isActivated, JsonElement &object)
{
    display.clear();

    if (isActivated)
    {
        display.printRegion(0, 0, object["err"].asString());
    }
    else
    {

        if (object["hCtrl"]["conn"].asString()[0] != 'A')
        {
            display.printRegion(0, 0, object["hCtrl"]["conn"].asString());
        }
        else if (strlen(object["err"].asString()) > 0)
        {
            display.printRegion(0, 0, "ERROR");
        }
        else
        {
            display.printRegion(0, 0, object["status"].asString());
        }

        display.printRegion(12, 0, "R");
        display.printBattery(13, 0, this->m_robotBatChar, object["bat"].asFloat() * 100);
        display.printRegion(14, 0, "C");
        display.printBattery(15, 0, this->m_controllerBatChar, 30.0);

        display.printRegion(0, 1, "A:");
        display.printRegion(2, 1, String(static_cast<int>(object["angle"].asFloat())));

        display.printRegion(5, 1, "P:");
        display.printRegion(7, 1, String(static_cast<int>(object["frPres"].asFloat())));

        display.printRegion(11, 1, "D:");
        display.printRegion(13, 1, String(object["vlvTm"].asInt()));
    }
}

void DashPage::clockwise(JsonElement &object)
{
    return;
}
void DashPage::counterClockwise(JsonElement &object)
{
    return;
}
