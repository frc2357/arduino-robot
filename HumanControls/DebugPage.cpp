#include "DebugPage.h"

DebugPage::DebugPage(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
                     unsigned int controllerBatChar)
    : Page(false, false, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::DEBUG_PAGE)
{
}

void DebugPage::paint(DisplayController &display, bool isActivated, TShirtCannonPayload &payload)
{
    display.clear();
    display.printRegion(0, 0, "batteryVoltage:");
    display.printRegion(2, 0, String(payload.m_batteryVoltage.asInt()));

    display.printRegion(0, 1, "messageIndex:");
    display.printRegion(2, 1, String(payload.m_messageIndex.asInt()));
}

void DebugPage::clockwise(TShirtCannonPayload &payload)
{
    return;
}
void DebugPage::counterClockwise(TShirtCannonPayload &payload)
{
    return;
}

int DebugPage::rangeFilter(int value)
{
    return 0;
}