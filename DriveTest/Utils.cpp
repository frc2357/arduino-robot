#include "Utils.h"

#define MAX_DRIVE_SPEED 63

void Utils::setMotors(TShirtCannonPayload &payload, float turn, float speed)
{
    double left, right;
    uint8_t leftSpeed, rightSpeed, leftDir, rightDir;

    right = (-1 * turn) + (-1 * speed);
    left = turn + (-1 * speed);

    leftDir = left < 0 ? 1 : 0;
    rightDir = right < 0 ? 1 : 0;

    leftSpeed = MAX_DRIVE_SPEED * abs(left);
    rightSpeed = MAX_DRIVE_SPEED * abs(right);

    if (leftSpeed > MAX_DRIVE_SPEED)
    {
        leftSpeed = MAX_DRIVE_SPEED;
    }
    else if (leftSpeed < 0)
    {
        leftSpeed = 0;
    }
    if (rightSpeed > MAX_DRIVE_SPEED)
    {
        rightSpeed = MAX_DRIVE_SPEED;
    }
    else if (rightSpeed < 0)
    {
        rightSpeed = 0;
    }

    payload.setControllerDriveLeft((leftDir << 6) | leftSpeed);
    payload.setControllerDriveRight((1 << 7) | (rightDir << 6) | rightSpeed);
}

void Utils::setAngle(TShirtCannonPayload &payload, int dir)
{
    // 2 = Increase angle, 1 = Decrease angle, 0 = Stay at the current angle

    if (dir == 1)
    {
        payload.setAngle(2);
    }
    else if (dir == -1)
    {
        payload.setAngle(1);
    }
    else
    {
        payload.setAngle(0);
    }
}
