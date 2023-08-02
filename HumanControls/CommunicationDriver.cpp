#include "CommunicationDriver.h"

CommunicationDriver::CommunicationDriver(LinkedList &messageQueue, int freq, int txPower, uint8_t rfm95_cs, uint8_t rfm95_int)
        : m_driver(rfm95_cs, rfm95_int), m_messageQueue(messageQueue)
{
    this->m_rfm95Freq = freq;
    this->m_rfm95TxPower = txPower;
}

void CommunicationDriver::connect()
{
    if (!m_driver.init())
    {
        Serial.println("Driver init failed");
        return;
    }

    if (!m_driver.setFrequency(m_rfm95Freq))
    {
        Serial.println("Could not set driver frequency");
        return;
    }

    m_driver.setTxPower(m_rfm95TxPower, false);

    this->m_isConnected = true;
}