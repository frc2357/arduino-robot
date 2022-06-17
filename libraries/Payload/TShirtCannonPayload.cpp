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
    m_batteryVoltage = 3;
    m_angle = 0;
    m_tankPressure = 0;
    m_firingPressure = 0;
    m_firingTime = 0;
}

uint8_t TShirtCannonPayload::buildTransmission()
{
    uint8_t sizes[PAYLOAD_LENGTH];
    uint8_t data[11];
    data[0] = m_messageType;
    sizes[0] = 2;
    data[1] = m_messageIndex;
    sizes[1] = 5;
    data[2] = m_status;
    sizes[2] = 3;
    data[3] = m_error;
    sizes[3] = 2;
    data[4] = m_controllerDriveLeft;
    sizes[4] = 8;
    data[5] = m_controllerDriveRight;
    sizes[5] = 8;
    data[6] = m_batteryVoltage;
    sizes[6] = 2;
    data[7] = m_angle;
    sizes[7] = 7;
    data[8] = m_tankPressure;
    sizes[8] = 7;
    data[9] = m_firingPressure;
    sizes[9] = 7;
    data[10] = m_firingTime;
    sizes[10] = 5;

    uint8_t transmission[PAYLOAD_LENGTH];

    int bitPos = 0;
    int elem = 0;
    for (int i = 0;i < DATA_LENGTH;i++) {
        uint8_t x = data[i];

        for (int j = 0;j < sizes[i];j++) {
            bitWrite(transmission[elem], bitPos, x & 1);
            bitPos++;
            x >>= 1;
            if (bitPos >= 8) {
                elem++;
                bitPos = 0;
            }
        }
    }

    Serial.println(transmission[0]);
    Serial.println(transmission[1]);
    Serial.println(transmission[2]);
    Serial.println(transmission[3]);
    Serial.println(transmission[4]);
    Serial.println(transmission[5]);
    Serial.println(transmission[6]);


    return transmission;
}

boolean TShirtCannonPayload::readMessage(uint8_t *message)
{
    return false;
}

void TShirtCannonPayload::print()
{
    // Serial.println();
    // Serial.print("Message Type: ");
    // Serial.println(m_payload->messageType, BIN);

    // Serial.print("Message Index: ");
    // Serial.println(m_payload->messageIndex, BIN);

    // Serial.print("Status: ");
    // Serial.println(m_payload->status, BIN);

    // Serial.print("Error: ");
    // Serial.println(m_payload->error, BIN);

    // Serial.print("Left Drive: ");
    // Serial.println(m_payload->controllerDriveLeft, BIN);

    // Serial.print("Right Drive: ");
    // Serial.println(m_payload->controllerDriveRight, BIN);

    // Serial.print("Battery Voltage: ");
    // Serial.println(m_payload->batteryVoltage, BIN);

    // Serial.print("Angle: ");
    // Serial.println(m_payload->angle, BIN);

    // Serial.print("Tank Pressure: ");
    // Serial.println(m_payload->tankPressure, BIN);

    // Serial.print("Firing Pressure: ");
    // Serial.println(m_payload->firingPressure, BIN);

    // Serial.print("Firing Time: ");
    // Serial.println(m_payload->firingTime, BIN);
    // Serial.println();
}