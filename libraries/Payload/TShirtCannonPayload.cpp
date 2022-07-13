#include "TShirtCannonPayload.h"

const int TShirtCannonPayload::PAYLOAD_LENGTH = 7;
const int TShirtCannonPayload::DATA_LENGTH = 11;

TShirtCannonPayload::TShirtCannonPayload()
{
    m_messageType = 0;
    m_messageIndex = 0;
    m_status = 0;
    m_error = 0;
    m_controllerDriveLeft = 0;
    m_controllerDriveRight = 0;
    m_batteryVoltage = 0;
    m_angle = 0;
    m_tankPressure = 0;
    m_firingPressure = 0;
    m_firingTime = 0;
}

bool TShirtCannonPayload::buildTransmission(uint8_t *transmission, uint8_t len)
{

    if (len < PAYLOAD_LENGTH)
    {
        Serial.println("Message is not long enough to hold data.");
        return false;
    }

    uint8_t data[DATA_LENGTH];
    data[0] = m_messageType;
    data[1] = m_messageIndex;
    data[2] = m_status;
    data[3] = m_error;
    data[4] = m_controllerDriveLeft;
    data[5] = m_controllerDriveRight;
    data[6] = m_batteryVoltage;
    data[7] = m_angle;
    data[8] = m_tankPressure;
    data[9] = m_firingPressure;
    data[10] = m_firingTime;

    Serial.println(data[0], BIN);

    int bitPos = 0;
    int elem = 0;
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        uint8_t attrVal = data[i];
        for (int j = 0; j < getAttributeSize(static_cast<AttributeSize>(i)); j++)
        {
            bitWrite(transmission[elem], bitPos, attrVal & 1);
            bitPos++;
            attrVal >>= 1;
            if (bitPos >= 8)
            {
                elem++;
                bitPos = 0;
            }
        }
    }

    return true;
}

bool TShirtCannonPayload::readMessage(uint8_t *message, uint8_t len)
{

    uint8_t data[DATA_LENGTH];
    memset(data, 0, sizeof(data));

    if (len < PAYLOAD_LENGTH)
    {
        Serial.println("Message is not long enough to fill in data.");
        return false;
    }

    int bitPos = 0;
    int elem = 0;

    uint8_t attrVal = message[0];
    for (int i = 0; i < DATA_LENGTH; i++)
    {
        Serial.print("i: ");
        Serial.println(i);
        for (int j = 0; j < getAttributeSize(static_cast<AttributeSize>(i)); j++)
        {
            bitWrite(data[i], j, attrVal & 1);
            attrVal >>= 1;
            bitPos++;
            if (bitPos >= 8)
            {
                bitPos = 0;
                elem++;
                attrVal = message[elem];
            }
        }
    }

    m_messageType = data[0];
    m_messageIndex = data[1];
    m_status = data[2];
    m_error = data[3];
    m_controllerDriveLeft = data[4];
    m_controllerDriveRight = data[5];
    m_batteryVoltage = data[6];
    m_angle = data[7];
    m_tankPressure = data[8];
    m_firingPressure = data[9];
    m_firingTime = data[10];

    return true;
}

uint8_t TShirtCannonPayload::getAttributeSize(AttributeSize attr)
{
    switch (attr)
    {
    case AttributeSize::MESSAGE_TYPE:
    case AttributeSize::BATTERY_VOLTAGE:
    case AttributeSize::ERROR:
        return 2;
    case AttributeSize::STATUS:
        return 3;
    case AttributeSize::MESSAGE_INDEX:
    case AttributeSize::FIRING_TIME:
        return 5;
    case AttributeSize::ANGLE:
    case AttributeSize::TANK_PRESSURE:
    case AttributeSize::FIRING_PRESSURE:
        return 7;
    case AttributeSize::CONTROLLER_DRIVE_LEFT:
    case AttributeSize::CONTROLLER_DRIVE_RIGHT:
        return 8;
    }
}

void TShirtCannonPayload::print()
{
    Serial.println();
    Serial.print("Message Type: ");
    Serial.println(m_messageType, BIN);

    Serial.print("Message Index: ");
    Serial.println(m_messageIndex, BIN);

    Serial.print("Status: ");
    Serial.println(m_status, BIN);

    Serial.print("Error: ");
    Serial.println(m_error, BIN);

    Serial.print("Left Drive: ");
    Serial.println(m_controllerDriveLeft, BIN);

    Serial.print("Right Drive: ");
    Serial.println(m_controllerDriveRight, BIN);

    Serial.print("Battery Voltage: ");
    Serial.println(m_batteryVoltage, BIN);

    Serial.print("Angle: ");
    Serial.println(m_angle, BIN);

    Serial.print("Tank Pressure: ");
    Serial.println(m_tankPressure, BIN);

    Serial.print("Firing Pressure: ");
    Serial.println(m_firingPressure, BIN);

    Serial.print("Firing Time: ");
    Serial.println(m_firingTime, BIN);
    Serial.println();
}

// Getters
uint8_t TShirtCannonPayload::getMessageType()
{
    return this->messageType;
}

uint8_t TShirtCannonPayload::getMessageIndex()
{
    return this->messageIndex;
}

uint8_t TShirtCannonPayload::getStatus()
{
    return this->status;
}

uint8_t TShirtCannonPayload::getError()
{
    return this->error;
}

uint8_t TShirtCannonPayload::getControllerDriveLeft()
{
    return this->controllerDriveLeft;
}

uint8_t TShirtCannonPayload::getControllerDriveRight()
{
    return this->controllerDriveRight;
}

uint8_t TShirtCannonPayload::getBatteryVoltage()
{
    return this->batteryVoltage;
}

uint8_t TShirtCannonPayload::getAngle()
{
    return this->angle;
}

uint8_t TShirtCannonPayload::getTankPressure()
{
    return this->tankPressure;
}

uint8_t TShirtCannonPayload::getFiringPressure()
{
    return this->firingPressure;
}

uint8_t TShirtCannonPayload::getFiringTime()
{
    return this->firingTime;
}

// Setters
void TShirtCannonPayload::setMessageType(uint8_t value)
{
    this->messageType = value;
}
void TShirtCannonPayload::setMessageIndex(uint8_t value)
{
    this->messageIndex = value;
}
void TShirtCannonPayload::setStatus(uint8_t value)
{
    this->status = value;
}
void TShirtCannonPayload::setError(uint8_t value)
{
    this->error = value;
}
void TShirtCannonPayload::setControllerDriveLeft(uint8_t value)
{
    this->controllerDriveLeft = value;
}
void TShirtCannonPayload::setControllerDriveRight(uint8_t value)
{
    this->controllerDriveRight = value;
}
void TShirtCannonPayload::setBatteryVoltage(uint8_t value)
{
    this->batteryVoltage = value;
}
void TShirtCannonPayload::setAngle(uint8_t value)
{
    this->angle = value;
}
void TShirtCannonPayload::setTankPressure(uint8_t value)
{
    this->tankPressure = value;
}
void TShirtCannonPayload::setFiringPressure(uint8_t value)
{
    this->firingPressure = value;
}
void TShirtCannonPayload::setFiringTime(uint8_t value)
{
    this->firingTime = value;
}