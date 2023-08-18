#include "CommunicationDriver.h"

CommunicationDriver::CommunicationDriver(TShirtCannonPayload &payload, LinkedList &messageQueue, boolean &isConnected, int freq, int txPower, uint8_t rfm95_cs, uint8_t rfm95_int)
        : m_driver(rfm95_cs, rfm95_int), m_messageQueue(messageQueue), m_payload(payload), m_isConnected(isConnected)
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

void CommunicationDriver::sendNextMessage() 
{
  uint8_t* data = m_messageQueue.pull();
  m_driver.send(data, sizeof(data));
}