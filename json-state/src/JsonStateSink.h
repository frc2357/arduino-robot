#ifndef JSONSTATESINK_H
#define JSONSTATESINK_H

#include <ArduinoJson.h>

class JsonStateSink {
    public:
      virtual void write(char *input, size_t inputLength) = 0;
};

#endif /* JSONSTATESINK_H */
