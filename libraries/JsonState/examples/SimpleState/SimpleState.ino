/*
 * Example of using JsonState for state tracking.
 */
#include <ArduinoJson.h>

#include <JsonState.h>

#define USB_BAUDRATE 115200

String name = "Test Devices";
String initialState = "{name: '" + name + "', devices: {example: {}}}";

JsonState jsonState(initialState, 1024);

JsonObject exampleDevice = jsonState.getState()["devices"]["example"];

double count = 0;
char buffer[1024];

void setup() {
  Serial.begin(USB_BAUDRATE);
}

void loop() {
  jsonState.updateField(exampleDevice, "count", count);

  Serial.println(jsonState.appendToString(String(), false));
  Serial.println(jsonState.getStringValue(jsonState.getState(), "name"));
  Serial.println(jsonState.getDoubleValue(exampleDevice, "count"));

  count += 0.5;
  delay(500);
}
