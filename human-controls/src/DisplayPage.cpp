#include "DisplayPage.h"

DisplayPage::DisplayPage()
{
    this->name = "";
    this->isActivate = false;
}

DisplayPage::DisplayPage(String name, bool isActivate)
{
    this->name = name;
    this->isActivate = isActivate;
    this->value = 0;
}

void DisplayPage::displayPageInit(String name, bool isActivate)
{
    this->name = name;
    this->isActivate = isActivate;
    this->value = 0;
}

DisplayPage::~DisplayPage()
{
}
void DisplayPage::cleanUp(DisplayController &display)
{
    display.clear();
}
void DisplayPage::paint(DisplayController &display, bool isActive)
{
    //Serial.println("painting to " + this->name);
    if (isActive & isActivate)
    {
        display.printRegion(8, 1, String(this->value));
    }
    display.printRegion(2, 0, this->name);
}
void DisplayPage::clockwise()
{
    this->value++;
}
void DisplayPage::counterClockwise()
{
    this->value--;
}
bool DisplayPage::canActivate()
{
    return this->isActivate;
}