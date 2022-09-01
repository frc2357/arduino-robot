#ifndef STATUS_ENABLED_H
#define STATUS_ENABLED_H

#include "RobotStatus.h"

class StatusEnabled : RobotStatus {
public:
    void update();
    void onTransition();
}