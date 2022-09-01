#ifndef STATUS_FIRING_H
#define STATUS_FIRING_H

#include "RobotStatus.h"

class StatusFiring : RobotStatus {
public:
    void update();
    void onTransition();
}