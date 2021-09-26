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
                               unsigned int durationMax,
                               unsigned int hangTimerDuration)
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

    this->m_hangTimerDuration = hangTimerDuration;
    this->m_time = millis();
}

void MenuController::init(JsonState &state)
{
    Serial.println("Menu Init");
    this->m_display.init();
    this->m_currentPage->paint(m_display, this->m_isActive, state);
}

void MenuController::menuRefresh(JsonState &state)
{
    this->m_currentPage->paint(m_display, m_isActive, state);
}

void MenuController::menuUpdate(JsonState &state)
{
    this->m_rotation = this->m_rotaryKnob.getValue();

    if (m_isActive & m_currentPage->canActivate())
    {
        if (this->m_rotation == 1)
        {
            this->m_currentPage->clockwise(state);
        }

        if (this->m_rotation == -1)
        {
            this->m_currentPage->counterClockwise(state);
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
        this->m_isActive = false;
    }

    if (this->m_rotation != 0)
    {
        this->m_time = millis();
        this->m_currentPage->cleanUp(m_display);
        this->m_currentPage->paint(m_display, m_isActive, state);
    }

    if (this->m_currentPage->getName() != m_dashPage.getName() && millis() > (this->m_time + this->m_hangTimerDuration))
    {
        this->m_currentPage = &m_dashPage;
        this->m_currentPage->cleanUp(m_display);
        this->m_currentPage->paint(m_display, m_isActive, state);
    }
}

void MenuController::menuPress(JsonState &state, bool isEnabled, FireController &fireController)
{
    this->m_isActive = !this->m_isActive;
    this->m_currentPage->paint(m_display, m_isActive, state);
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