#include "JsonState.h"

JsonState::JsonState(String initialState, JsonStateSink *sink)
  : JsonState::JsonState(initialState, sink, JSON_DEFAULT_STATE_MAX_LEN)
{}

JsonState::JsonState(String initialState, JsonStateSink *sink, size_t stateMaxLen) {
  this->stateMaxLen = stateMaxLen;
  this->stateStringBuffer = new char[stateMaxLen];
  this->state = new DynamicJsonDocument(stateMaxLen);
  this->sink = sink;
  deserializeJson(*this->state, initialState);
}

JsonState::~JsonState() {
  delete(this->stateStringBuffer);
  delete(this->state);
}

JsonDocument *JsonState::getState() {
  return this->state;
}

void JsonState::sendState() {
  size_t written = serializeJson(*this->state, this->stateStringBuffer, stateMaxLen);
  this->sink->write(this->stateStringBuffer, written);
}

void JsonState::write(char *input, size_t inputSize) {
  StaticJsonDocument<1024> message;
  DeserializationError readErr = deserializeJson(message, input, inputSize);

  if (DeserializationError::Ok != readErr) {
    setError("JSON Parse", readErr.c_str());
    sendState();
    return;
  }

  JsonObject fields = message.as<JsonObject>();

  updateObject(this->state->as<JsonObject>(), fields);
  clearError();
  sendState();
}

void JsonState::updateField(JsonObject stateObject, String fieldName, String value) {
  StaticJsonDocument<JSON_STRING_LEN> fields;
  deserializeJson(fields, "{" + fieldName + ": '" + value + "'}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateField(JsonObject stateObject, String fieldName, double value) {
  StaticJsonDocument<JSON_DOUBLE_LEN> fields;
  String valueStr(value);
  deserializeJson(fields, "{" + fieldName + ": " + value + "}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateField(JsonObject stateObject, String fieldName, int value) {
  StaticJsonDocument<JSON_INT_LEN> fields;
  String valueStr(value);
  deserializeJson(fields, "{" + fieldName + ": " + valueStr + "}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateField(JsonObject stateObject, String fieldName, bool value) {
  StaticJsonDocument<JSON_BOOL_LEN> fields;
  String valueStr = value ? "true" : "false";
  deserializeJson(fields, "{" + fieldName + ": " + valueStr + "}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateObject(JsonObject stateObject, JsonObject fields) {
  for (JsonPair kv : fields) {
    const String key = kv.key().c_str();
    const JsonVariant value = kv.value();
    JsonVariant existingValue = stateObject[key];

    if (value.is<JsonObject>() &&
        existingValue.is<JsonObject>() &&
        !existingValue.isNull()) {

      // Only replace the fields given by recursing a level deeper.
      updateObject(existingValue, value);
    } else {
      // Overwrite the entire object
      stateObject[key] = value;
    }
  }
}

void JsonState::setError(String type, String message) {
  StaticJsonDocument<512> error;
  deserializeJson(error, "{type: '" + type + "', message: '" + message + "'}");
  (*this->state)["error"] = error.as<JsonObject>();
}

void JsonState::clearError() {
  this->state->as<JsonObject>().remove("error");
}
