#include "Page.h"

Page::Page()
{
    this->isActivate = false;
    this->modifyValue = 0;
    increment = 0;
    this->min = 0;
    this->max = 0;
}

Page::Page(int increment, int min, int max, bool isActivate, int printersLength)
{
    this->isActivate = isActivate;
    this->modifyValue = min;
    this->min = min;
    this->max = max;
    this->increment = increment;
    this->printersLength = printersLength;
}

void Page::cleanUp(DisplayController &display)
{
    display.clear();
}
void Page::paint(DisplayController &display, bool isActive)
{
    display.clear();
    /* for (int i = 0; i < this->printersLength; i++)
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
    }*/
    display.printRegion(2, 0, "On a page");
    Serial.println("On a page");
}

void Page::clockwise()
{
    if (this->modifyValue < this->max)
    {
        this->modifyValue += this->increment;
    }
}
void Page::counterClockwise()
{
    if (this->modifyValue > this->min)
    {
        this->modifyValue -= this->increment;
    }
}
bool Page::canActivate()
{
    return this->isActivate;
}

void Page::setNextPage(Page &page)
{
    previousPage = &page;
}
void Page::setPreviousPage(Page &page)
{
    nextPage = &page;
}
Page *Page::getNextPage()
{
    return nextPage;
}
Page *Page::getPreviousPage()
{
    return previousPage;
}