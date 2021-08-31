#include "FireController.h"

FireController::FireController()
{
    this->isFireToggled = false;
}

bool FireController::initiateFiring(bool isReady)
{
    if (isReady && this->isFireToggled)
    {
        Serial.println("FIRE");
        return true;
    }
    else
    {
        return false;
    }
}

void FireController::setIsFireToggled(bool value)
{
    this->isFireToggled = value;
}

bool FireController::getIsFireToggled()
{
    return this->isFireToggled;
}
