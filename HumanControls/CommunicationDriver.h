#ifndef COMMUNICATION_DRIVER_H
#define COMMUNICATION_DRIVER_H

#include "RFM95C.h"
#include "LinkedList.h"
#include "TShirtCannonPayload.h"

class CommunicationDriver
{
public:
  CommunicationDriver(TShirtCannonPayload &payload, LinkedList &messageQueue, boolean &isConnected, int freq, int txPower, uint8_t rfm95_cs, uint8_t rfm95_int);
  void connect();
  void sendNextMessage();

private:
  TShirtCannonPayload m_payload;

  bool m_isConnected;
  int m_rfm95Freq, m_rfm95TxPower;
  RFM_95C m_driver;

  LinkedList m_messageQueue;
};

#endif