#include "DashPage.h"

DashPage::DashPage(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
                   unsigned int controllerBatChar) : Page(true, false, downArrow, upArrow, robotBatChar, controllerBatChar,
                                                          Page::PageType::DASH_PAGE)
{
}

void DashPage::paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload)
{
    display.clear();

    if (isActivated)
    {
        display.printRegion(0, 0, "ERROR: " + String(payload.getError()));
    }
    else
    {
        if (payload.getError() != 0)
        {
            display.printRegion(0, 0, "ERROR");
        }
        else
        {
            display.printRegion(0, 0, "Status: " + String(payload.getStatus()));
            Serial.println(payload.getStatus());
        }

        display.printRegion(12, 0, "R");
        display.printBattery(13, 0, this->m_robotBatChar, payload.getBatteryVoltage() * 100.0);
        display.printRegion(14, 0, "C");
        display.printBattery(15, 0, this->m_controllerBatChar, 30.0);

        display.printRegion(0, 1, "A:");
        display.printRegion(2, 1, String(payload.getAngle()));

        display.printRegion(5, 1, "P:");
        display.printRegion(7, 1, String(payload.getFiringPressure()));

        display.printRegion(11, 1, "D:");
        display.printRegion(13, 1, String(payload.getFiringTime()));
    }
}

void DashPage::clockwise(TShirtCannonPayload &payload)
{
    return;
}
void DashPage::counterClockwise(TShirtCannonPayload &payload)
{
    return;
}

int DashPage::rangeFilter(int value)
{
    return 0;
}