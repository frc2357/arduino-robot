#ifndef ROBOT_STATUS_H
#define ROBOT_STATUS_H

class Robot;

class RobotStatus {
public:
    void setRobot(Robot *robot);
    virtual void update(); // What to do every loop

private:
    virtual void validateState(); // Checks validity of called state, method will transition to another if necessary
    virtual void robotAction(); // Implements what the robot does in a given state

protected:
    Robot *m_robot;
};

#endif