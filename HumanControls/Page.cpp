#include "Page.h"

Page::Page(unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
           unsigned int controllerBatChar, Page::PageType name)
{
    this->m_isActive = false;
    this->m_name = name;

    this->m_downArrow = downArrow;
    this->m_upArrow = upArrow;
    this->m_robotBatChar = robotBatChar;
    this->m_controllerBatChar = controllerBatChar;
}

Page::Page(bool isActive, unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
           unsigned int controllerBatChar, Page::PageType name)
{
    this->m_isActive = isActive;
    this->m_name = name;

    this->m_downArrow = downArrow;
    this->m_upArrow = upArrow;
    this->m_robotBatChar = robotBatChar;
    this->m_controllerBatChar = controllerBatChar;
}

void Page::cleanUp(DisplayController &display)
{
    display.clear();
}

bool Page::canActivate()
{
    return this->m_isActive;
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