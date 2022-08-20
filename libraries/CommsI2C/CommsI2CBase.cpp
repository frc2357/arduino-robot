#include "CommsI2CBase.h"

const size_t CommsI2CBase::I2C_MAX_BYTES = 32;
const uint8_t CommsI2CBase::PREAMBLE_VALUE = 255;

CommsI2CBase::CommsI2CBase(int myAddress, int preambleLength) {
  m_myAddress = myAddress;
  m_preambleLength = preambleLength;
}

void CommsI2CBase::init() {
  Wire.begin(m_myAddress);
}

int CommsI2CBase::fillBuffer(uint8_t *buf, const uint8_t len) {
  int bytesRead = 0;
  while(available() && len < bytesRead) {
    buf[bytesRead] = read(); 
    bytesRead++;
  }
  return bytesRead;
}

const bool CommsI2CBase::parseBuffer(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen) {
  int preambleCount = 0;
  int dataPos = 0;

  if(bufLen < datLen + m_preambleLength) {
    return false;
  }

  // Find start of data
  for( ; (dataPos < bufLen) || (preambleCount == 4); dataPos++) {
    if (buf[dataPos] == PREAMBLE_VALUE) {
      preambleCount++;
    } else {
      preambleCount = 0;
    }
  }

  // Validate preamble is found, and there is enough in buffer for it to be good data
  if((preambleCount != 4) || (bufLen - dataPos < datLen)) {
    return false;
  }

  // Verify data is complete
  int secondPreambleStart = 0;

  preambleCount = 0;
  for(int i = dataPos; (i < bufLen); i++) {
    if (buf[i] == PREAMBLE_VALUE) {
      preambleCount++;
    } else {
      preambleCount = 0;
    }

    if(preambleCount == 4) {
      secondPreambleStart = i-4;
      break;
    }
  }

  if((secondPreambleStart > 0) && (secondPreambleStart - dataPos < datLen)) {
    return false;
  }  

  memcpy(data, (buf + dataPos), datLen);
  return true;
}

const bool CommsI2CBase::getBytes(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen) {
  int bytesRead = fillBuffer(buf, bufLen);

  return parseBuffer(buf, bytesRead, data, datLen);
}

int CommsI2CBase::available() {
  return Wire.available();
}

// Read a single byte from the buffer
int CommsI2CBase::read() {
  return Wire.read();
}

// Peek next byte
int CommsI2CBase::peek() {
  return Wire.peek();
}