#include "FireController.h"

FireController::FireController()
{
    this->m_isPrimed = false;
}

bool FireController::initiateFiring(bool isReady)
{
    if (isReady && this->m_isPrimed)
    {
        Serial.println("FIRE");
        return true;
    }
    else
    {
        return false;
    }
}

void FireController::setIsPrimed(bool value)
{
    this->m_isPrimed = value;
}

bool FireController::getIsPrimed()
{
    return this->m_isPrimed;
}
