#include "MenuController.h"

MenuController::MenuController(unsigned int encoderPinClk, unsigned int encoderPinDt,
                               unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth,
                               unsigned int menuSize)
{
    this->rotaryKnob = new RotaryKnobController(encoderPinClk, encoderPinDt);
    this->display = new DisplayController(displayAddress, displayLen, displayWidth);

    this->menuSize = menuSize;
    this->isActive = false;

    this->rotation = -1;
    this->menuIndex = 0;
    this->lastMenuIndex = 0;
}

void MenuController::menuInit(DisplayPage displayPages[])
{
    this->displayPages = displayPages;
    this->display->displayInit();
    this->displayPages[menuIndex].paint(*display, this->isActive);
    Serial.print("Menu Init");
}

MenuController::~MenuController()
{
    delete (this->rotaryKnob);
    delete (this->display);
}

void MenuController::menuUpdate()
{
    this->rotation = this->rotaryKnob->getValue();
    //Serial.println(this->rotation);
    //this->rotaryKnob->getValue();
    if (isActive & this->displayPages[menuIndex].canActivate())
    {
        if (this->rotation == 1)
        {
            this->displayPages[menuIndex].clockwise();
            this->displayPages[menuIndex].paint(*display, isActive);
        }

        if (this->rotation == -1)
        {
            this->displayPages[menuIndex].counterClockwise();
            this->displayPages[menuIndex].paint(*display, isActive);
        }
    }
    else
    {
        if (this->rotation == 1)
        {
            this->menuIndex = ((this->menuIndex + 1) < this->menuSize) ? this->menuIndex + 1 : 0;
        }

        if (this->rotation == -1)
        {
            this->menuIndex = ((this->menuIndex - 1) >= 0) ? this->menuIndex - 1 : (menuSize - 1);
        }

        if (menuIndex != lastMenuIndex)
        {
            this->displayPages[lastMenuIndex].cleanUp(*display);
            lastMenuIndex = menuIndex;
            this->displayPages[menuIndex].paint(*display, isActive);
        }
        this->isActive = false;
    }
}

void MenuController::menuPress()
{
    this->isActive = !this->isActive;
    this->displayPages[menuIndex].paint(*display, isActive);
}