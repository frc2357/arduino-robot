#ifndef JSONSTATE_H
#define JSONSTATE_H

#define JSON_DEFAULT_STATE_MAX_LEN 1024
#define JSON_STRING_LEN  512
#define JSON_INT_LEN     64
#define JSON_DOUBLE_LEN  128
#define JSON_BOOL_LEN    64

#include "Arduino.h"
#include <ArduinoJson.h>
#include "JsonStateSink.h"

class JsonState : public JsonStateSink {
  public:
    JsonState(String initialState, JsonStateSink *sink);
    JsonState(String initialState, JsonStateSink *sink, size_t stateMaxLen);
    ~JsonState();

    JsonDocument *getState();

    void write(char *input, size_t inputLength);

    void sendState();
    void updateField(JsonObject stateObject, String fieldName, String value);
    void updateField(JsonObject stateObject, String fieldName, int value);
    void updateField(JsonObject stateObject, String fieldName, double value);
    void updateField(JsonObject stateObject, String fieldName, bool value);
    void updateObject(JsonObject stateObject, JsonObject fields);
    void setError(String type, String message);
    void clearError();

  private:
    size_t stateMaxLen;
    char *stateStringBuffer;
    DynamicJsonDocument *state;
    JsonStateSink *sink;
};

#endif /* jsonstate_h */
