#include "TShirtCannonPayload.h"

const int TShirtCannonPayload::PAYLOAD_LENGTH = 7;

uint8_t *TShirtCannonPayload::buildTransmission()
{

    uint8_t transmission[7] = {};
    return transmission;
}

boolean TShirtCannonPayload::readMessage(uint8_t *message)
{
    return false;
}

void TShirtCannonPayload::print()
{ /*
     Serial.println();
     Serial.println("Message Type: " + char(m_payload.messageType));
     Serial.println("Message Index: " + static_cast<char>(m_payload.messageIndex));
     Serial.println("Status: " + static_cast<char>(m_payload.status));
     Serial.println("Error: " + static_cast<char>(m_payload.error));
     Serial.println("Left Drive: " + static_cast<char>(m_payload.controllerDriveLeft));
     Serial.println("Right Drive: " + static_cast<char>(m_payload.controllerDriveRight));
     Serial.println("Battery Voltage: " + static_cast<char>(m_payload.battery_voltage));
     Serial.println("Angle: " + static_cast<char>(m_payload.angle));
     Serial.println("Tank Pressure: " + static_cast<char>(m_payload.tank_pressure));
     Serial.println("Firing Pressure: " + static_cast<char>(m_payload.firing_pressure));
     Serial.println("Firing Time: " + static_cast<char>(m_payload.firing_time));
     Serial.println();*/
}