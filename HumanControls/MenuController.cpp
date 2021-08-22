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
    : rotaryKnob(encoderPinClk, encoderPinDt), display(displayAddress, displayLen, displayWidth),
      dashPage(), elevatorPage(angleIncrement, angleMin, angleMax),
      shotPage(pressureIncrement, pressureMin, pressureMax),
      valvePage(durationIncrement, durationMin, durationMax)
{
    this->isActive = false;

    this->rotation = 0;

    //Set previous pages
    dashPage.setPreviousPage(valvePage);
    elevatorPage.setPreviousPage(dashPage);
    shotPage.setPreviousPage(elevatorPage);
    valvePage.setPreviousPage(shotPage);

    //Set next pages
    dashPage.setNextPage(elevatorPage);
    elevatorPage.setNextPage(shotPage);
    shotPage.setNextPage(valvePage);
    valvePage.setNextPage(dashPage);

    currentPage = &dashPage;
}

void MenuController::init()
{
    Serial.println("Menu Init");
    this->display.init();
    this->currentPage->paint(display, this->isActive);
}

void MenuController::menuUpdate()
{
    this->rotation = this->rotaryKnob.getValue();

    if (isActive & currentPage->canActivate())
    {
        if (this->rotation == 1)
        {
            this->currentPage->clockwise();
            this->currentPage->paint(display, isActive);
        }

        if (this->rotation == -1)
        {
            this->currentPage->counterClockwise();
            this->currentPage->paint(display, isActive);
        }
    }
    else
    {
        if (this->rotation == 1)
        {
            this->currentPage = this->currentPage->getNextPage();
        }

        if (this->rotation == -1)
        {
            this->currentPage = this->currentPage->getPreviousPage();
        }

        if (this->rotation != 0)
        {
            this->currentPage->cleanUp(display);
            this->currentPage->paint(display, isActive);
        }
        this->isActive = false;
    }
}

void MenuController::menuPress()
{
    this->isActive = !this->isActive;
    this->currentPage->paint(display, isActive);
}