#ifndef JSONSTATE_H
#define JSONSTATE_H

#define JSONSTATE_DEFAULT_STATE_MAX_LEN 1024
#define JSONSTATE_STRING_LEN  512
#define JSONSTATE_INT_LEN     64
#define JSONSTATE_DOUBLE_LEN  128
#define JSONSTATE_BOOL_LEN    64

#include "Arduino.h"
#include <ArduinoJson.h>

class JsonState {
  public:
    static const String Ok;

    JsonState(String initialState);
    JsonState(String initialState, size_t stateMaxLen);

    JsonObject getState();

    String appendToString(String str, bool pretty);
    void writeToSerial(bool pretty);

    String updateFromString(String str);
    String updateFromSerial();

    String getStringValue(JsonObject stateObject, String fieldName);
    int getIntValue(JsonObject stateObject, String fieldName);
    double getDoubleValue(JsonObject stateObject, String fieldName);
    bool getBooleanValue(JsonObject stateObject, String fieldName);

    void updateField(JsonObject stateObject, String fieldName, String value);
    void updateField(JsonObject stateObject, String fieldName, int value);
    void updateField(JsonObject stateObject, String fieldName, double value);
    void updateField(JsonObject stateObject, String fieldName, bool value);
    void updateObject(JsonObject stateObject, JsonObject fields);

  private:
    DynamicJsonDocument document;
};

#endif /* jsonstate_h */
