/*
 * JsonStateSerialTest
 */
#include <ArduinoJson.h>

#include "JsonState.h"
#include "JsonStateSerial.h"

#define USB_BAUDRATE     115200

String name = "Test Devices";
String initialState = "{name: '" + name + "', devices: {example: {}}}";

JsonStateSerial jsonSerial;

JsonState jsonState(initialState, &jsonSerial);

JsonObject exampleDevice = (*jsonState.getState())["devices"]["example"];

double count = 0;

void setup() {
  jsonSerial.initialize(USB_BAUDRATE, &jsonState);
  jsonState.sendState();
}

void loop() {
  jsonSerial.handleSerial();
  delay(500);

  count += 0.5;

  jsonState.updateField(exampleDevice, "count", count);
  jsonState.sendState();
}
