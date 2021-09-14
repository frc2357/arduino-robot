#include "Page.h"

Page::Page(Page::PageType name)
{
    this->m_isActive = false;
    this->m_modifyValue = 0;
    m_increment = 0;
    this->m_min = 0;
    this->m_max = 0;
    this->m_name = name;
}

Page::Page(int increment, int min, int max, bool isActive, Page::PageType name)
{
    this->m_isActive = isActive;
    this->m_modifyValue = min;
    this->m_min = min;
    this->m_max = max;
    this->m_increment = increment;
    this->m_name = name;
}

void Page::cleanUp(DisplayController &display)
{
    display.clear();
}

void Page::clockwise()
{
    if (this->m_modifyValue < this->m_max)
    {
        this->m_modifyValue += this->m_increment;
    }
}
void Page::counterClockwise()
{
    if (this->m_modifyValue > this->m_min)
    {
        this->m_modifyValue -= this->m_increment;
    }
}
bool Page::canActivate()
{
    return this->m_isActive;
}

int Page::getModifyValue()
{
    return this->m_modifyValue;
}

void Page::setNextPage(Page &page)
{
    m_nextPage = &page;
}
void Page::setPreviousPage(Page &page)
{
    m_previousPage = &page;
}
Page::PageType Page::getName()
{
    return m_name;
}
Page *Page::getNextPage()
{
    return m_nextPage;
}
Page *Page::getPreviousPage()
{
    return m_previousPage;
}