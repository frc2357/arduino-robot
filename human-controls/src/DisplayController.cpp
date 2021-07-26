#include "DisplayController.h"

DisplayController::DisplayController(unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth)
{
    this->lcd = new LiquidCrystal_I2C(displayAddress, displayLen, displayWidth);
}

DisplayController::~DisplayController()
{
    delete (this->lcd);
}

void DisplayController::displayInit()
{
    this->lcd->begin();
    this->lcd->backlight();
    this->lcd->clear();
}

void DisplayController::clear()
{
    this->lcd->clear();
}

void DisplayController::printRegion(int x, int y, String text)
{
    this->lcd->setCursor(x, y);
    this->lcd->print(text);
}