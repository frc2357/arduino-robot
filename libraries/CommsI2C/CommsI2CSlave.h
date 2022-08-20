#ifndef COMMSI2CSLAVE_H
#define COMMSI2CSLAVE_H

#include <Arduino.h>
#include <Wire.h>
#include "CommsI2CBase.h"

#define BUFFER_LEN 7

class CommsI2CSlave : public CommsI2CBase {
    public:
    static const uint8_t BUFFER_LEN;

    CommsI2CSlave(int myAddress, int preambleLength);

    void init();
    void update(const uint8_t *data, const uint8_t len);

    void sendBytes();
    const bool getBytes();

    size_t write(uint8_t ch);
    
  private:
    uint8_t m_datLen;
    uint8_t* m_data;

    uint8_t buffer[BUFFER_LEN]
};

#endif