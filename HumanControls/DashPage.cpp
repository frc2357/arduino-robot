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

        if (!object["hCtl"]["conn"].asString())
        {
            display.printRegion(0, 0, "Disconnected");
        }
        else if (strlen(object["err"].asString()) > 0)
        {
            display.printRegion(0, 0, "ERROR");
        }
        else
        {
            display.printRegion(0, 0, object["status"].asString());
            Serial.println(object["status"].asString());
        }

        display.printRegion(12, 0, "R");
        display.printBattery(13, 0, this->m_robotBatChar, object["b"].asFloat() * 100);
        display.printRegion(14, 0, "C");
        display.printBattery(15, 0, this->m_controllerBatChar, 30.0);

        display.printRegion(0, 1, "A:");
        display.printRegion(2, 1, String(object["ang"].asInt()));

        display.printRegion(5, 1, "P:");
        display.printRegion(7, 1, String(object["fPr"].asInt()));

        display.printRegion(11, 1, "D:");
        display.printRegion(13, 1, String(object["vlv"].asInt()));
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