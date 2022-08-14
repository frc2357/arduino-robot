#include "CommsI2C.h"
#include <Wire.h>

const size_t CommsI2C::I2C_MAX_BYTES = 32;
const uint8_t CommsI2C::PREAMBLE_VALUE = 255;

CommsI2C::CommsI2C(int myAddress, int deviceAddress, int preambleLength) {
  m_myAddress = myAddress;
  m_deviceAddress = deviceAddress;
  m_preambleLength = preambleLength;
}

void CommsI2C::init() {
  Wire.begin(m_myAddress);
}

void CommsI2C::beginWrite() {
  m_totalBytesWritten = 0;
  m_messageBytesWritten = 0;
  Wire.beginTransmission(m_deviceAddress);
}

size_t CommsI2C::endWrite() {
  Wire.endTransmission();
  return m_totalBytesWritten;
}

void CommsI2C::sendBytes(const uint8_t *data, const uint8_t len) {
  beginWrite();

  // Write preamble
  for (int i = 0; i < m_preambleLength; i++) {
    write(PREAMBLE_VALUE);
  }

  // Write data
  for(int i = 0; i < len; i++){
    write(data[i]);
  }

  size_t bytesWritten = endWrite();
}

int CommsI2C::fillBuffer(uint8_t *buf, const uint8_t len) {
  int bytesRead = 0;
  while(available() && len < bytesRead) {
    buf[bytesRead] = read(); 
    bytesRead++;
  }
  return bytesRead;
}

void CommsI2C::parseBuffer(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen) {
  int preambleCount = 0;
  int dataPos = 0;

  if(bufLen < datLen + m_preambleLength) {
    return;
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
    return;
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
    return;
  }  

  memcpy(data, (buf + dataPos), datLen);
}

void CommsI2C::getBytes(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen) {
  int bytesRead = fillBuffer(buf, bufLen);

  parseBuffer(buf, bytesRead, data, datLen);
}


int CommsI2C::available() {
  return Wire.available();
}

// Read a single byte from the buffer
int CommsI2C::read() {
  return Wire.read();
}

// Peek next byte
int CommsI2C::peek() {
  return Wire.peek();
}

size_t CommsI2C::write(uint8_t value) {
  if (m_messageBytesWritten == I2C_MAX_BYTES) {
    // We're at max message size. Stop this one and start the next.
    Wire.endTransmission();
    Wire.beginTransmission(m_deviceAddress);
    m_messageBytesWritten = 0;
  }
  Wire.write(value);
  m_messageBytesWritten++;
  m_totalBytesWritten++;
}
