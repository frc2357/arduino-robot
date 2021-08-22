#include "RotaryKnobController.h"

RotaryKnobController::RotaryKnobController(unsigned int pinClk, unsigned int pinDt)
{
    // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
    this->encoder = new RotaryEncoder(pinClk, pinDt, RotaryEncoder::LatchMode::TWO03);

    this->pos = 0;
}

RotaryKnobController::~RotaryKnobController()
{
    delete (this->encoder);
}

int RotaryKnobController::getValue()
{
    this->encoder->tick();
    int newPos = this->encoder->getPosition();
    //Dials
    if (pos != newPos && newPos % 2 == 0)
    {
        pos = newPos;

        //COUNTERCLOCKWISE
        if (this->encoder->getDirection() == RotaryEncoder::Direction::CLOCKWISE)
        {
            return -1;
        }

        //CLOCKWISE
        else if (this->encoder->getDirection() == RotaryEncoder::Direction::NOROTATION)
        {
            return 1;
        }
    }
    return 0;
}