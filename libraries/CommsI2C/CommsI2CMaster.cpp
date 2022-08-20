#include "CommsI2CMaster.h"
#include <Wire.h>

CommsI2CMaster::CommsI2CMaster(int myAddress, int deviceAddress, int preambleLength) :  CommsI2CBase(myAddress, deviceAddress){
  m_deviceAddress = deviceAddress;
}

void CommsI2CMaster::sendBytes(const uint8_t *data, const uint8_t len) {
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

const bool CommsI2CMaster::getBytes(uint8_t *buf, const uint8_t bufLen, uint8_t *data, const uint8_t datLen) {
  requestData(datLen);
  
  return CommsI2CBase::getBytes(buf, bufLen, data, datLen);
}

void CommsI2CMaster::beginWrite() {
  m_totalBytesWritten = 0;
  m_messageBytesWritten = 0;
  Wire.beginTransmission(m_deviceAddress);
}

size_t CommsI2CMaster::endWrite() {
  Wire.endTransmission();
  return m_totalBytesWritten;
}

void CommsI2CMaster::requestData(const uint8_t datLen) {
  Wire.requestFrom(m_deviceAddress, datLen);
}

size_t CommsI2CMaster::write(uint8_t value) {
  if (m_messageBytesWritten == I2C_MAX_BYTES) {
    // We're at max message size. Stop this one and start the next.
    Wire.endTransmission();
    Wire.beginTransmission(m_deviceAddress);
    m_messageBytesWritten = 0;
  }
  Wire.write(value);
  m_messageBytesWritten++;
  m_totalBytesWritten++;
  return m_messageBytesWritten;
}
