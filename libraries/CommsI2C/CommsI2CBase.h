#ifndef COMMSI2CBASE_H
#define COMSSI2CBASE_H

#include <Arduino.h>
#include <Wire.h>

class CommsI2CBase : public Stream {
    protected:
        static const size_t I2C_MAX_BYTES;
        static const uint8_t PREAMBLE_VALUE;

        CommsI2CBase(int myAddress, int preambleLength);

        virtual void init();
        int fillBuffer(uint8_t *buf, const uint8_t len);
        const bool parseBuffer(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen);

        virtual int available();
        virtual int read();
        virtual int peek();

        virtual const bool getBytes(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen);
        int m_myAddress;
        size_t m_preambleLength;
        size_t m_messageBytesWritten;
        size_t m_totalBytesWritten;
};

#endif