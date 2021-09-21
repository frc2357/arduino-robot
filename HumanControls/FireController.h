#ifndef FIRE_CONTROLLER_H
#define FIRE_CONTROLLER_H

#include "Arduino.h"

class FireController
{
public:
    FireController();
    bool initiateFiring(bool isReady);
    void setIsFireToggled(bool value);
    bool getIsFireToggled();

private:
    bool m_isFireToggled;
};

#endif