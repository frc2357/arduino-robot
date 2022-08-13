#ifndef COMMSI2C_H
#define COMMSI2C_H

#include <Arduino.h>
#include <JsonElement.h>

class CommsI2C : public Stream {
  public:
    static const size_t I2C_MAX_BYTES;
    static const uint8_t PREAMBLE_VALUE;

    CommsI2C(int myAddress, int deviceAddress, int preambleLength);

    void init();
    void sendBytes(const uint8_t *data, const uint8_t len);
    int fillBuffer(uint8_t *buf, const uint8_t len);
    void parseBuffer(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen);
    void getBytes(const uint8_t bufLen, uint8_t *data, const uint8_t datLen);

    void beginWrite();
    size_t endWrite();

    virtual int available();
    virtual int read();
    virtual int peek();
    virtual size_t write(uint8_t ch);

  private:
    int m_myAddress;
    int m_deviceAddress;
    size_t m_preambleLength;
    size_t m_messageBytesWritten;
    size_t m_totalBytesWritten;
};

#endif // COMMSI2C_H
