#include "MenuController.h"

MenuController::MenuController(unsigned int encoderPinClk, unsigned int encoderPinDt, unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth)
{
    this->rotaryKnob = new RotaryKnobController(encoderPinClk, encoderPinDt);
    this->display = new DisplayController(displayAddress, displayLen, displayWidth);
    this->rotation = -1;
    this->menuIndex = 0;
    this->menuSize = 4;
    this->lastMenuIndex = 0;
    this->isActive = false;
    this->displayPages = new DisplayPage[this->menuSize];
    this->displayPages[0].displayPageInit("Dash", false);
    this->displayPages[1].displayPageInit("Elevator Angle", true);
    this->displayPages[2].displayPageInit("Shot Pressure", true);
    this->displayPages[3].displayPageInit("Valve Duration", true);
}

MenuController::~MenuController()
{
    delete (this->rotaryKnob);
    delete (this->display);
}

void MenuController::menuInit()
{
    this->display->displayInit();
    this->displayPages[menuIndex].paint(*display, isActive);
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
}