#include "DisplayController.h"

DisplayController::DisplayController(unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth)
    : m_lcd(displayAddress, displayLen, displayWidth)
{
}

void DisplayController::init()
{
    this->m_lcd.begin();
    this->m_lcd.backlight();
    this->m_lcd.clear();
}

void DisplayController::clear()
{
    this->m_lcd.clear();
}

void DisplayController::printRegion(int x, int y, String text)
{
    this->m_lcd.setCursor(x, y);
    this->m_lcd.print(text);
}