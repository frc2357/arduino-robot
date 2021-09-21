#include "EnableController.h"

EnableController::EnableController()
{
    this->m_isEnabled = false;
}

bool EnableController::getIsEnabled()
{
    return this->m_isEnabled;
}

void EnableController::setIsEnabled(bool value)
{
    this->m_isEnabled = value;
}
