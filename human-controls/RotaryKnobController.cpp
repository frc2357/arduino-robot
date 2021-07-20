#include "RotaryKnobController.h"

RotaryKnobController::RotaryKnobController(unsigned int pinClk, unsigned int pinDt)
{
    // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
    this->encoder = new RotaryEncoder(pinClk, pinDt, RotaryEncoder::LatchMode::TWO03);
}

/*RotaryKnobController::RotaryKnobController(unsigned int pinClk, unsigned int pinDt, void (MenuController::*clockwisePtr)(), void (MenuController::*counterClockwisePtr)(), void (MenuController::*menuSelectPtr)())
{
    // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
    this->encoder = new RotaryEncoder(pinClk, pinDt, RotaryEncoder::LatchMode::TWO03);
    // this->clockwisePtr = &clockwisePtr;
    //this->counterClockwisePtr = &counterClockwisePtr;
    //this->menuSelectPtr = &menuSelectPtr;
}*/

RotaryKnobController::~RotaryKnobController()
{
    delete (this->encoder);
}

void RotaryKnobController::updateRotaryKnob()
{
    this->encoder->tick();
    int newPos = this->encoder->getPosition();
    //Dial
    if (pos != newPos && newPos % 2 == 0)
    {

        if (this->encoder->getDirection() == RotaryEncoder::Direction::CLOCKWISE)
        {
            counterClockwise();
        }
        else if (this->encoder->getDirection() == RotaryEncoder::Direction::NOROTATION)
        {
            clockwise();
        }

        pos = newPos;
    }
}

void RotaryKnobController::clockwise()
{
    //this->*clockwisePtr();
    Serial.print("Up");
}
void RotaryKnobController::counterClockwise()
{
    //this->*counterClockwisePtr();
    Serial.print("Down");
}
void RotaryKnobController::menuSelect()
{
    //this->*menuSelectPtr();
    Serial.print("Select");
}