#include "Page.h"

Page::Page(bool isActive, bool applyHang, unsigned int downArrow, unsigned int upArrow, unsigned int robotBatChar,
           unsigned int controllerBatChar, Page::PageType name)
{
    this->m_isActive = isActive;
    this->m_applyHang = applyHang;
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

bool Page::applyHang()
{
    return this->m_applyHang;
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