#include "Utils.h"

size_t Utils::incrementRingBufferIndex(size_t index, size_t length)
{
  return index = index < length - 1 ? index + 1 : 0;
}

int Utils::binaryToServoPWM(uint8_t value)
{
  int dir = value & 64;

  int speed = value & 63;

  int mappedSpeed = map(speed, 0, 63, 0, 90);

  return 90 + (dir == 64 ? -mappedSpeed : mappedSpeed);
}