#include "CommsI2C.h"
#include <Wire.h>

const size_t CommsI2C::I2C_MAX_BYTES = 32;
const char *CommsI2C::JSON_PREAMBLE = "~~~";

CommsI2C::CommsI2C(int myAddress, int deviceAddress) {
  m_myAddress = myAddress;
  m_deviceAddress = deviceAddress;
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

void CommsI2C::sendState(JsonState &state) {
  beginWrite();
  this->print(JSON_PREAMBLE);
  endWrite();

  beginWrite();
  state.printJson(*this, true);
  state.clearChanged();
  endWrite();
}

// TODO: Fill out for receiving data
int CommsI2C::available() {
  return 0;
}

// TODO: Fill out for receiving data
int CommsI2C::read() {
  return 0;
}

// TODO: Fill out for receiving data
int CommsI2C::peek() {
  return 0;
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
