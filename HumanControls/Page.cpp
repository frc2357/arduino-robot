#include "Page.h"

Page::Page(Page::PageType name)
{
    this->isActive = false;
    this->modifyValue = 0;
    increment = 0;
    this->min = 0;
    this->max = 0;
    this->name = name;
}

Page::Page(int increment, int min, int max, bool isActive, Page::PageType name)
{
    this->isActive = isActive;
    this->modifyValue = min;
    this->min = min;
    this->max = max;
    this->increment = increment;
    this->name = name;
}

void Page::cleanUp(DisplayController &display)
{
    display.clear();
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
    return this->isActive;
}

int Page::getModifyValue()
{
    return this->modifyValue;
}

void Page::setNextPage(Page &page)
{
    nextPage = &page;
}
void Page::setPreviousPage(Page &page)
{
    previousPage = &page;
}
Page::PageType Page::getName()
{
    return name;
}
Page *Page::getNextPage()
{
    return nextPage;
}
Page *Page::getPreviousPage()
{
    return previousPage;
}