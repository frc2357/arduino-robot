#ifndef COMMUNICATION_DRIVER_H
#define COMMUNICATION_DRIVER_H

#include "RFM95C.h"
#include "LinkedList.h"
#include "TShirtCannonPayload.h"
#include "Utils.h"

class CommunicationDriver
{
public:
  CommunicationDriver(LinkedList &messageQueue, int freq, int txPower, uint8_t rfm95_cs, uint8_t rfm95_int);
  void connect(TShirtCannonPayload &payload);
  void sendNextMessage();

private:
  int m_rfm95Freq, m_rfm95TxPower;
  RFM_95C m_driver;

  LinkedList m_messageQueue;
};

#endif