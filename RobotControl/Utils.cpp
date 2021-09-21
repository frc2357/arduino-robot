#include "Utils.h"

size_t Utils::incrementRingBufferIndex(size_t index, size_t length) {
  return index = index < length - 1 ? index + 1 : 0;
}
