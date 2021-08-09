#include "JsonState.h"

const String JsonState::Ok = "OK";

JsonState::JsonState(String initialState)
  : JsonState::JsonState(initialState, JSONSTATE_DEFAULT_STATE_MAX_LEN)
{}

JsonState::JsonState(String initialState, size_t stateMaxLen)
  : document(DynamicJsonDocument(stateMaxLen))
{
  deserializeJson(this->document, initialState);
}

JsonObject JsonState::getState() {
  return this->document.as<JsonObject>();
}

String JsonState::appendToString(String str, bool pretty) {
  if (pretty) {
    serializeJsonPretty(this->document, str);
  } else {
    serializeJson(this->document, str);
  }
  return str;
}

void JsonState::writeToSerial(bool pretty) {
  if (pretty) {
    serializeJsonPretty(this->document, Serial);
  } else {
    serializeJson(this->document, Serial);
  }
}

String JsonState::updateFromString(String str) {
  StaticJsonDocument<JSONSTATE_DEFAULT_STATE_MAX_LEN> message;
  DeserializationError readErr = deserializeJson(message, str);

  if (DeserializationError::Ok != readErr) {
    return readErr.c_str();
  }

  JsonObject fields = message.as<JsonObject>();
  updateObject(this->document.as<JsonObject>(), fields);
  return JsonState::Ok;
}

String JsonState::updateFromSerial() {
  return this->updateFromString(Serial.readString());
}

String JsonState::getStringValue(JsonObject stateObject, String fieldName) {
  JsonVariant variant = stateObject[fieldName];
  return variant.as<String>();
}

int JsonState::getIntValue(JsonObject stateObject, String fieldName) {
  JsonVariant variant = stateObject[fieldName];
  return variant.as<int>();
}

double JsonState::getDoubleValue(JsonObject stateObject, String fieldName) {
  JsonVariant variant = stateObject[fieldName];
  return variant.as<double>();
}

bool JsonState::getBooleanValue(JsonObject stateObject, String fieldName) {
  JsonVariant variant = stateObject[fieldName];
  return variant.as<bool>();
}

void JsonState::updateField(JsonObject stateObject, String fieldName, String value) {
  StaticJsonDocument<JSONSTATE_STRING_LEN> fields;
  deserializeJson(fields, "{" + fieldName + ": '" + value + "'}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateField(JsonObject stateObject, String fieldName, double value) {
  StaticJsonDocument<JSONSTATE_DOUBLE_LEN> fields;
  String valueStr(value);
  deserializeJson(fields, "{" + fieldName + ": " + value + "}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateField(JsonObject stateObject, String fieldName, int value) {
  StaticJsonDocument<JSONSTATE_INT_LEN> fields;
  String valueStr(value);
  deserializeJson(fields, "{" + fieldName + ": " + valueStr + "}");
  updateObject(stateObject, fields.as<JsonObject>());
}

void JsonState::updateField(JsonObject stateObject, String fieldName, bool value) {
  StaticJsonDocument<JSONSTATE_BOOL_LEN> fields;
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
