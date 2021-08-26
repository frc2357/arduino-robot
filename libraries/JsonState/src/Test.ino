/*
 * Test
 */
#include "JsonEl.h"

#define USB_BAUDRATE     115200

JsonEl sensor1Fields[] = { JsonEl("name", "sensor1"), JsonEl("value", 0.25) };
JsonEl sensor2Fields[] = { JsonEl("name", "sensor2"), JsonEl("value", 5.8) };

JsonEl valuesArray[] = { 0.1, 0.25, 0.5 };

JsonEl stateFields[] = {
  JsonEl("name", "controller name"),
  JsonEl("version", "1.0.0"),
  JsonEl("tick", 0),
  JsonEl("sensor1", sensor1Fields, sizeof(sensor1Fields) / sizeof(sensor1Fields[0])),
  JsonEl("sensor2", sensor2Fields, sizeof(sensor2Fields) / sizeof(sensor2Fields[0])),
  JsonEl("values", valuesArray, sizeof(valuesArray) / sizeof(valuesArray[0])),
};
JsonEl state(stateFields, sizeof(stateFields) / sizeof(stateFields[0]));


JsonEl array[] = { JsonEl(0.25), JsonEl("test") };
JsonEl el(array, sizeof(array) / sizeof(array[0]));

JsonEl values("values", valuesArray, sizeof(valuesArray) / sizeof(valuesArray[0]));

void setup() {
  Serial.begin(USB_BAUDRATE);
}

void loop() {
  state.PrintJson(Serial);
  Serial.println();
  delay(5000);

  if (Serial.available() > 0) {
    int valuesUpdated = state.UpdateFromJson(Serial.readString());
    Serial.println(String(valuesUpdated) + " values updated");
  }

  //Serial.println(state["tick"].AsInt());
  //state["tick"] = state["tick"].AsInt() + 1;
}
