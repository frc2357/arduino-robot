#ifndef JSONSTATESERIAL_H
#define JSONSTATESERIAL_H

#include "JsonStateSink.h"

#define JSON_SERIAL_DEFAULT_STATE_MAX_LEN 1024

class JsonStateSerial : public JsonStateSink {
  public:
    JsonStateSerial();
    JsonStateSerial(size_t stateMaxLen);
    ~JsonStateSerial();

    void initialize(unsigned long baudRate, JsonStateSink *output);

    void handleSerial();
    void write(char *input, size_t inputLength);

  private:
    size_t stateMaxLen;
    char *stateStringBuffer;
    JsonStateSink *output;
};

#endif /* JSONSTATESERIAL_H */
