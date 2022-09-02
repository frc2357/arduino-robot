#ifndef STATUS_ADJUSTING_H
#define STATUS_ADJUSTING_H

#include "RobotStatus.h"
#include "StatusEnum.h"

class StatusAdjusting : public RobotStatus {
public:
    void update();
    void onTransition();
};

#endif