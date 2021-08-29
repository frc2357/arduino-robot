#ifndef FIRE_CONTROLLER_H
#define FIRE_CONTROLLER_H

#include "Arduino.h"

class FireController
{
public:
    FireController();
    bool initiateFiring(bool isReady);
    void setIsFireToggled(bool value);
    void toggleIsFireToggled();
    bool getIsFireToggled();

private:
    bool isFireToggled;
};

#endif