#ifndef ROBOT_STATUS_H
#define ROBOT_STATUS_H

class Robot;

class RobotStatus {
public:
    void setRobot(Robot *robot);
    virtual void update(); // What to do every loop
    virtual void onTransition(); // What to do when switching to given status

protected:
    Robot *m_robot;

};

#endif