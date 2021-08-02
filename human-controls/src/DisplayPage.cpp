#include "DisplayPage.h"

DisplayPage::DisplayPage()
{
    this->isActivate = false;
    this->modifyValue = 0;
    increment = 0;
}

DisplayPage::DisplayPage(int increment, bool isActivate, int printersLength, Printer printers[])
{
    this->isActivate = isActivate;
    this->modifyValue = 0;
    this->increment = increment;
    this->printersLength = printersLength;
    this->printers = printers;
}

void DisplayPage::displayPageInit(int increment, bool isActivate, int printersLength, Printer printers[])
{
    this->isActivate = isActivate;
    this->modifyValue = 0;
    this->increment = increment;
    this->printersLength = printersLength;
    this->printers = printers;
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
    display.clear();
    //Serial.println("painting to " + this->name);
    /*if (isActive & isActivate)
    {
        display.printRegion(8, 1, String(this->modifyValue));
    }
    display.printRegion(2, 0, this->name);*/
    for (int i = 0; i < this->printersLength; i++)
    {
        Serial.println(this->printers[i].getText());
        if (isActive & isActivate & this->printers[i].isActiveText())
        {
            display.printRegion(this->printers[i].getX(), this->printers[i].getY(), this->printers[i].getText());
        }
        else if (this->printers[i].isActiveText() == false)
        {
            if (this->printers[i].getText() == "!modifyValue!")
            {
                display.printRegion(this->printers[i].getX(), this->printers[i].getY(), String(this->modifyValue));
            }
            else
            {
                display.printRegion(this->printers[i].getX(), this->printers[i].getY(), this->printers[i].getText());
            }
        }
    }
}

void DisplayPage::clockwise()
{
    this->modifyValue += this->increment;
}
void DisplayPage::counterClockwise()
{
    this->modifyValue -= this->increment;
}
bool DisplayPage::canActivate()
{
    return this->isActivate;
}