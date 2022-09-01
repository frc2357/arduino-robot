#ifndef ROBOT_STATUS_H
#define ROBOT_STATUS_H

#include "Robot.h"

class RobotStatus {
public:
    void setRobot(Robot *robot);
    virtual void update() = 0; // What to do every loop
    virtual void onTransition() = 0; // What to do when switching to given status

protected:
    Robot *m_robot;

};

#endif