#include "CommsI2CSlave.h"

CommsI2CMaster::CommsI2CMaster(int myAddress, int deviceAddress, int preambleLength) :  CommsI2CBase(myAddress, deviceAddress){
  m_deviceAddress = deviceAddress;
}

 void CommsI2CSlave::init() {
    CommsI2CBase::init();

    // Set up events
 }

 void CommsI2CSlave::update(const uint8_t *data, const uint8_t len) {
    if(len >= m_datLen) {
        memcpy(m_data, data, m_datLen);
    }
 }


void CommsI2Cslave::sendBytes() {
  // Write preamble
  for (int i = 0; i < m_preambleLength; i++) {
    Wire.write(PREAMBLE_VALUE);
  }

  // Write data
  for(int i = 0; i < len; i++){
    Wire.write(data[i]);
  }
}

const bool CommsI2Cslave::getBytes() {  
  return CommsI2CBase::getBytes(buf, bufLen, data, datLen);
}