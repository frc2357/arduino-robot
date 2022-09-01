#ifndef STATUS_PRIMED_H
#define STATUS_PRIMED_H

#include "RobotStatus.h"

class StatusPrimed : RobotStatus {
public:
    void update();
    void onTransition();
}