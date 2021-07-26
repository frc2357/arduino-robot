/*
 * JsonStateSerialTest
 */
#include <ArduinoJson.h>

#include "JsonState.h"

#define USB_BAUDRATE     115200

String name = "Test Devices";
String initialState = "{name: '" + name + "', devices: {example: { count: 0 }}}";

JsonState jsonState(initialState, 1024);

JsonObject exampleDevice = jsonState.getState()["devices"]["example"];

void setup() {
  Serial.begin(USB_BAUDRATE);
}

void loop() {
  jsonState.writeToSerial(true);
  Serial.println();

  if (Serial.available() > 0) {
    String result = jsonState.updateFromSerial();
    if (result != JsonState::Ok) {
      Serial.println(result);
    }
  }

  double count = jsonState.getDoubleValue(exampleDevice, "count");
  jsonState.updateField(exampleDevice, "count", count + 0.5);
  delay(500);
}
