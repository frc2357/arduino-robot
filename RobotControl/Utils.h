#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

class Utils
{
public:
  static size_t incrementRingBufferIndex(size_t index, size_t length);
  static int binaryToServoPWM(uint8_t value);
};

#endif // UTILS_H
