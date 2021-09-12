#include "FireController.h"

FireController::FireController()
{
    this->m_isFireToggled = false;
}

bool FireController::initiateFiring(bool isReady)
{
    if (isReady && this->m_isFireToggled)
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
    this->m_isFireToggled = value;
}

bool FireController::getIsFireToggled()
{
    return this->m_isFireToggled;
}
