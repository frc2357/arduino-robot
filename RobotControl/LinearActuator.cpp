#include "LinearActuator.h"

const int LinearActuator::RETRACT = 0;
const int LinearActuator::STOP = 1;
const int LinearActuator::EXTEND = 2;

const int LinearActuator::RETRACT_WRITE_VALUE = 0;
const int LinearActuator::STOP_WRITE_VALUE = 90;
const int LinearActuator::EXTEND_WRITE_VALUE = 180;

const int LinearActuator::MAX_POS_MILLIS = 8955;
const int LinearActuator::MIN_POS_MILLIS = 455;

LinearActuator::LinearActuator(int input)
{
  m_inputPin = input;
  m_currentDirection = STOP;
}

void LinearActuator::init()
{
  m_currentPos = 0;
  m_actuator.attach(m_inputPin, 1000, 2000);
}

void LinearActuator::update(int direction)
{
  if (m_currentDirection != direction)
  {
    m_currentDirection = direction;

    if (direction == EXTEND)
    {
      extend();
    }
    else if (direction == RETRACT)
    {
      retract();
    }
    else
    {
      stop();
    }
  }

  unsigned long currentMillis = millis();

  if (direction == EXTEND)
  {
    m_currentPos += currentMillis - m_lastMillis;
  }
  else if (direction == RETRACT)
  {
    m_currentPos -= currentMillis - m_lastMillis;
  }
  m_lastMillis = currentMillis;

  // if (m_currentPos < MIN_POS_MILLIS | m_currentPos > MAX_POS_MILLIS)
  // {
  //     stop();
  // }

  // Serial.print("Position: ");
  // Serial.println(m_currentPos);
}

bool LinearActuator::isMoving(int direction)
{
  return (direction == EXTEND || direction == RETRACT) || m_lastMillis > 0;
}

void LinearActuator::extend()
{
  m_lastMillis = millis();
  m_actuator.write(EXTEND_WRITE_VALUE);
  Serial.println("Extending...");
}

void LinearActuator::retract()
{
  m_lastMillis = millis();
  m_actuator.write(RETRACT_WRITE_VALUE);
  Serial.println("Retracting...");
}

void LinearActuator::stop()
{
  m_currentDirection = STOP;
  m_lastMillis = 0;
  m_actuator.write(STOP_WRITE_VALUE);
  Serial.println("Stopping...");
}