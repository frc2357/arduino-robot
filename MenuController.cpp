#include "MenuController.h"

MenuController::MenuController(unsigned int encoderPinClk, unsigned int encoderPinDt, unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth)
{
    this->rotaryKnob = new RotaryKnobController(encoderPinClk, encoderPinClk /*, this->clockwise, this->counterClockwise, this->menuSelect*/);
    this->display = new DisplayController(displayAddress, displayLen, displayWidth);
}

MenuController::~MenuController()
{
    delete (this->rotaryKnob);
    delete (this->display);
}

void MenuController::menuInit()
{
    this->display->displayInit();
}

void MenuController::clockwisedate()
{
    this->rotaryKnob->updateRotaryKnob();
    this->display->displayUpdate();
}

void MenuController::menuPress()
{
    this->rotaryKnob->menuSelect();
}

void MenuController::clockwise()
{
    this->display->clockwise();
}
void MenuController::counterClockwise()
{
    this->display->counterClockwise();
}
void MenuController::menuSelect()
{
    this->display->menuSelect();
}
