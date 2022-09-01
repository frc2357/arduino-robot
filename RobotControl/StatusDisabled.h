#ifndef STATUS_DISABLED_H
#define STATUS_DISABLED_H

#include "RobotStatus.h"

class StatusDisabled : RobotStatus {
public:
    void update();
    void onTransition();
}