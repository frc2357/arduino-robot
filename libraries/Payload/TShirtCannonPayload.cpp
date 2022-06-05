#include "TShirtCannonPayload.h"

const int TShirtCannonPayload::PAYLOAD_LENGTH = 7;

uint8_t *TShirtCannonPayload::buildTransmission()
{

    uint8_t *transmission = (uint8_t *)m_payload;
    return transmission;
}

boolean TShirtCannonPayload::readMessage(uint8_t *message)
{
    m_payload = (Payload *)message;
    return false;
}

void TShirtCannonPayload::print()
{
    Serial.println();
    Serial.print("Message Type: ");
    Serial.println(m_payload->messageType);

    Serial.print("Message Index: ");
    Serial.println(m_payload->messageIndex);

    Serial.print("Status: ");
    Serial.print(m_payload->status);

    Serial.print("Error: ");
    Serial.println(m_payload->error);

    Serial.print("Left Drive: ");
    Serial.println(m_payload->controllerDriveLeft);

    Serial.print("Right Drive: ");
    Serial.println(m_payload->controllerDriveRight);

    Serial.print("Battery Voltage: ");
    Serial.println(m_payload->batteryVoltage);

    Serial.print("Angle: ");
    Serial.println(m_payload->angle);

    Serial.print("Tank Pressure: ");
    Serial.println(m_payload->tankPressure);

    Serial.print("Firing Pressure: ");
    Serial.println(m_payload->firingPressure);

    Serial.print("Firing Time: ");
    Serial.println(m_payload->firingTime);
    Serial.println();
}