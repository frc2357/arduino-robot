#include "RotaryKnobController.h"

RotaryKnobController::RotaryKnobController(unsigned int pinClk, unsigned int pinDt)
{
    // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
    this->encoder = new RotaryEncoder(pinClk, pinDt, RotaryEncoder::LatchMode::TWO03);
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
            menuDown();
        }
        else if (this->encoder->getDirection() == RotaryEncoder::Direction::NOROTATION)
        {
            menuUp();
        }

        pos = newPos;
    }
}

void RotaryKnobController::menuUp()
{
    Serial.println("up");
}
void RotaryKnobController::menuDown()
{
    Serial.println("down");
}
void RotaryKnobController::menuSelect()
{
    Serial.println("select");
}