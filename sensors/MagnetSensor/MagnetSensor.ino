#define hallSensor 2
#include "JsonElement.h"
int oldState;

JsonElement sensorFields[] = {
    Json::Int("state", 0)};

JsonElement devices[] = {
    Json::Object("hallSensor", sensorFields)};

JsonElement deviceFields[] = {
    Json::String("name", "turretArdiuno"),
    Json::Object("devices", devices)};

JsonElement device = Json::Object(deviceFields);

JsonState JState(device);


void setup() {
  Serial.begin(115200);
  pinMode(hallSensor, INPUT);
  
}

void loop() {
  
  int currentState = digitalRead(hallSensor);
  if(currentState != oldState){
    JState.root()["devices"]["hallSensor"]["state"] = currentState;
    JState.printJson(Serial);
    Serial.println("");
    oldState = currentState;
  }
}
