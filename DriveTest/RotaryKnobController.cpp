#include "RotaryKnobController.h"

const int RotaryKnobController::VALVE_TIME_ADJUST_MODE = 0;
const int RotaryKnobController::ANGLE_ADJUST_MODE = 1;
const int RotaryKnobController::MIN_MODE_VALUE = VALVE_TIME_ADJUST_MODE;
const int RotaryKnobController::MAX_MODE_VALUE = ANGLE_ADJUST_MODE;

RotaryKnobController::RotaryKnobController(unsigned int pinClk, unsigned int pinDt)
{
    // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
    this->m_encoder = new RotaryEncoder(pinClk, pinDt, RotaryEncoder::LatchMode::TWO03);

    this->m_pos = 0;
}

RotaryKnobController::~RotaryKnobController()
{
    delete (this->m_encoder);
}

int RotaryKnobController::getValue()
{
    this->m_encoder->tick();
    int newPos = this->m_encoder->getPosition();
    //Dials
    if (m_pos != newPos && newPos % 2 == 0)
    {
        m_pos = newPos;

        //COUNTERCLOCKWISE
        if (this->m_encoder->getDirection() == RotaryEncoder::Direction::CLOCKWISE)
        {
            return -1;
        }

        //CLOCKWISE
        else if (this->m_encoder->getDirection() == RotaryEncoder::Direction::NOROTATION)
        {
            return 1;
        }
    }
    return 0;
}

void RotaryKnobController::nextMode() 
{
    this->m_mode++;

    if (this->m_mode > MAX_MODE_VALUE) 
    {
        this->m_mode = MIN_MODE_VALUE;
    }
}

int RotaryKnobController::getMode()
{
    return this->m_mode;
}