#include "EnableController.h"

EnableController::EnableController()
{
    this->isEnabled = false;
}

bool EnableController::getIsEnabled()
{
    return this->isEnabled;
}

void EnableController::setIsEnabled(bool value)
{
    this->isEnabled = value;
}
