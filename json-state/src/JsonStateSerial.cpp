#include "JsonStateSerial.h"

JsonStateSerial::JsonStateSerial()
 : JsonStateSerial::JsonStateSerial(JSON_SERIAL_DEFAULT_STATE_MAX_LEN)
{}

JsonStateSerial::JsonStateSerial(size_t stateMaxLen) {
  this->stateMaxLen = stateMaxLen;
  this->stateStringBuffer = new char[stateMaxLen];
  this->output = NULL;
}

JsonStateSerial::~JsonStateSerial() {
  delete(this->stateStringBuffer);
}

void JsonStateSerial::initialize(unsigned long baudRate, JsonStateSink *output) {
  Serial.begin(baudRate);
  this->output = output;
}

void JsonStateSerial::handleSerial() {
  if (Serial.available() > 0) {
    Serial.println("--- handleSerial");
    size_t charsRead = Serial.readBytes(stateStringBuffer, this->stateMaxLen);
    this->output->write(stateStringBuffer, charsRead);
  }
}

void JsonStateSerial::write(char *input, size_t inputLength) {
  Serial.write(input, inputLength);
  Serial.println();
}
