#ifndef COMMSI2CSLAVE_H
#define COMMSI2CSLAVE_H

#include <Arduino.h>
#include <Wire.h>
#include "CommsI2CBase.h"

#define BUFFER_LEN 32

class CommsI2CSlave : public CommsI2CBase {
    public:
    CommsI2CSlave(int myAddress, int preambleLength, uint8_t* data, uint8_t datLen);

    void init(void (*recFunction)(int), void (*reqFunction)(void));
    void updateData(uint8_t *data, const uint8_t datLen);

    void sendBytes();
    const bool getBytes();

    size_t write(uint8_t ch);
  private:
    uint8_t m_datLen;
    uint8_t* m_data;

    uint8_t m_buffer[BUFFER_LEN];
};

#endif