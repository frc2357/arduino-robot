#ifndef STATUS_DISABLED_H
#define STATUS_DISABLED_H

#include "RobotStatus.h"
#include "StatusEnum.h"

class StatusDisabled : public RobotStatus {
public:
    void update();
    void onTransition();
};

#endif