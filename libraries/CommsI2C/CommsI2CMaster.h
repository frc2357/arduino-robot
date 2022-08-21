#ifndef COMMSI2CMASTER_H
#define COMMSI2CMASTER_H

#include <Arduino.h>
#include <Wire.h>
#include "CommsI2CBase.h"

class CommsI2CMaster : public CommsI2CBase {
  public:
    CommsI2CMaster(int myAddress, int deviceAddress, int preambleLength);
    void init();

    void sendBytes(const uint8_t *data, const uint8_t len);
    const bool getBytes(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen);

    void beginWrite();
    size_t endWrite();
    void requestData(const uint8_t datLen);

    size_t write(uint8_t ch);
  private:
    int m_deviceAddress;
    size_t m_messageBytesWritten;
    size_t m_totalBytesWritten;
};

#endif // COMMSI2C_H
