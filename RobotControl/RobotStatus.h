#ifndef ROBOT_STATUS_H
#define ROBOT_STATUS_H

class Robot;

class RobotStatus {
public:
    void setRobot(Robot *robot);
    virtual void update(); // What to do to the robot every loop
    virtual void validateState(); // Checks validity of called state, method will transition to another if necessary

protected:
    Robot *m_robot;
};

#endif