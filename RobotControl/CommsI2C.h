#ifndef COMMSI2C_H
#define COMMSI2C_H

#include <Arduino.h>
#include <JsonElement.h>

class CommsI2C : public Stream {
  public:
    static const size_t I2C_MAX_BYTES;

    CommsI2C(int myAddress, int deviceAddress);

    void init();
    void sendBytes(const uint8_t *data, const uint8_t len);

    void beginWrite();
    size_t endWrite();

    virtual int available();
    virtual int read();
    virtual int peek();
    virtual size_t write(uint8_t ch);

  private:
    int m_myAddress;
    int m_deviceAddress;
    size_t m_messageBytesWritten;
    size_t m_totalBytesWritten;
};

#endif // COMMSI2C_H
