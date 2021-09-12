#include <Arduino.h>
#include "Log.h"

void Log::error(const char *format, ...) {
  va_list args;
  char message[64];
  va_start(args, format);
  vsprintf(message, format, args);
  va_end(args);

  Serial.print("ERROR: ");
  Serial.println(message);
}
