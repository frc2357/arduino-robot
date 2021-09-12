#include "MenuController.h"

MenuController::MenuController(unsigned int encoderPinClk,
                               unsigned int encoderPinDt,
                               unsigned int displayAddress,
                               unsigned int displayLen,
                               unsigned int displayWidth,
                               unsigned int angleIncrement,
                               unsigned int angleMin,
                               unsigned int angleMax,
                               unsigned int pressureIncrement,
                               unsigned int pressureMin,
                               unsigned int pressureMax,
                               unsigned int durationIncrement,
                               unsigned int durationMin,
                               unsigned int durationMax)
    : m_rotaryKnob(encoderPinClk, encoderPinDt), m_display(displayAddress, displayLen, displayWidth),
      m_dashPage(), m_elevatorPage(angleIncrement, angleMin, angleMax),
      m_shotPage(pressureIncrement, pressureMin, pressureMax),
      m_valvePage(durationIncrement, durationMin, durationMax)
{
    this->m_isActive = false;

    this->m_rotation = 0;

    //Set previous pages
    m_dashPage.setPreviousPage(m_valvePage);
    m_elevatorPage.setPreviousPage(m_dashPage);
    m_shotPage.setPreviousPage(m_elevatorPage);
    m_valvePage.setPreviousPage(m_shotPage);

    //Set next pages
    m_dashPage.setNextPage(m_elevatorPage);
    m_elevatorPage.setNextPage(m_shotPage);
    m_shotPage.setNextPage(m_valvePage);
    m_valvePage.setNextPage(m_dashPage);

    m_currentPage = &m_dashPage;
}

void MenuController::init(const char *status)
{
    Serial.println("Menu Init");
    this->m_display.init();
    this->m_currentPage->paint(m_display, this->m_isActive, status);
}

void MenuController::menuRefresh(const char *status)
{
    this->m_currentPage->paint(m_display, m_isActive, status);
}

void MenuController::menuUpdate(const char *status)
{
    this->m_rotation = this->m_rotaryKnob.getValue();

    if (m_isActive & m_currentPage->canActivate())
    {
        if (this->m_rotation == 1)
        {
            this->m_currentPage->clockwise();
            this->m_currentPage->paint(m_display, m_isActive, status);
        }

        if (this->m_rotation == -1)
        {
            this->m_currentPage->counterClockwise();
            this->m_currentPage->paint(m_display, m_isActive, status);
        }
    }
    else
    {
        if (this->m_rotation == 1)
        {
            this->m_currentPage = this->m_currentPage->getNextPage();
        }

        if (this->m_rotation == -1)
        {
            this->m_currentPage = this->m_currentPage->getPreviousPage();
        }

        if (this->m_rotation != 0)
        {
            this->m_currentPage->cleanUp(m_display);
            this->m_currentPage->paint(m_display, m_isActive, status);
        }
        this->m_isActive = false;
    }
}

void MenuController::menuPress(const char *status, bool isEnabled, FireController &fireController)
{
    this->m_isActive = !this->m_isActive;
    this->m_currentPage->paint(m_display, m_isActive, status);
    if (this->m_currentPage->getName() == m_dashPage.getName())
    {
        if (isEnabled)
        {
            fireController.setIsFireToggled(true);
        }
        else
        {
            fireController.setIsFireToggled(false);
        }
    }
}