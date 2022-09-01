#ifndef STATUS_ADJUSTING_H
#define STATUS_ADJUSTING_H

#include "RobotStatus.h"

class StatusAdjusting : RobotStatus {
public:
    void update();
    void onTransition();
}