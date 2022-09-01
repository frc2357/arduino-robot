#ifndef STATUS_PRIMED_H
#define STATUS_PRIMED_H

#include "RobotStatus.h"
#include "StatusEnum.h"

class StatusPrimed : RobotStatus {
public:
    void update();
    void onTransition();
}