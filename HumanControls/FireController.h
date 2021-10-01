#ifndef FIRE_CONTROLLER_H
#define FIRE_CONTROLLER_H

#include <Arduino.h>

class FireController
{
public:
    FireController();
    bool initiateFiring(bool isReady);
    void setIsPrimed(bool value);
    bool getIsPrimed();

private:
    bool m_isPrimed;
};

#endif