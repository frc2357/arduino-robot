#include "DebugPage.h"

DebugPage::DebugPage(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
                     unsigned int controllerBatChar)
    : Page(false, false, downArrow, upArrow, robotBatChar, controllerBatChar, Page::PageType::DEBUG_PAGE)
{
}

void DebugPage::paint(DisplayController &display, bool isActivated, JsonElement &object)
{
    display.clear();
    display.printRegion(0, 0, "v:");
    display.printRegion(2, 0, object["ver"].asString());

    display.printRegion(8, 0, "up:");
    display.printRegion(11, 0, String(object["up"].asInt()));

    display.printRegion(0, 1, "t:");
    display.printRegion(2, 1, String(object["tck"].asInt()));

    display.printRegion(9, 1, "avg:");
    display.printRegion(13, 1, String(object["avgTck"].asInt()));
}

void DebugPage::clockwise(JsonElement &object)
{
    return;
}
void DebugPage::counterClockwise(JsonElement &object)
{
    return;
}
