#include "MenuController.h"

MenuController::MenuController(unsigned int encoderPinClk, unsigned int encoderPinDt,
                               unsigned int displayAddress, unsigned int displayLen, unsigned int displayWidth,
                               unsigned int menuSize)
    : rotaryKnob(encoderPinClk, encoderPinDt), display(displayAddress, displayLen, displayWidth),
      dashPage(0, 0, 0, false, 0), elevatorPage(0, 0, 0, false, 0),
      shotPage(0, 0, 0, false, 0), valvePage(0, 0, 0, false, 0)
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
            Serial.println("Clocwise");
        }

        if (this->rotation == -1)
        {
            this->currentPage = this->currentPage->getPreviousPage();
            Serial.println("CounterClocwise");
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