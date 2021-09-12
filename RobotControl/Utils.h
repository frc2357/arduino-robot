#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class Utils {
  public:
    static size_t incrementRingBufferIndex(size_t index, size_t length);
};

#endif // UTILS_H
