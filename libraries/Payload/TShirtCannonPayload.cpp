#include "TShirtCannonPayload.h"

const int TShirtCannonPayload::PAYLOAD_LENGTH = 7;

TShirtCannonPayload::TShirtCannonPayload()
{
    memset(&m_payload, 0, sizeof(m_payload));
}

uint8_t *TShirtCannonPayload::buildTransmission()
{

    uint8_t *transmission = (uint8_t *)m_payload;
    return transmission;
}

boolean TShirtCannonPayload::readMessage(uint8_t *message)
{
    m_payload = (Payload)message;
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