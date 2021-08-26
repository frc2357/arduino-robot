#include "JsonEl.h"

JsonEl JsonEl::NotFound;
JsonEl JsonEl::NoKey("(no key)", "");

Print *JsonEl::errorLog = &Serial;

Print &JsonEl::GetErrorLogger() {
  return *errorLog;
}

void JsonEl::SetErrorLogger(Print &log) {
  errorLog = &log;
}

void JsonEl::LogError(const char *format, ...) {
  va_list args;
  char message[64];
  va_start(args, format);
  vsprintf(message, format, args);
  va_end(args);

  errorLog->print("[JSON ERROR: ");
  errorLog->print(message);
  errorLog->println("]");
}

JsonEl::JsonEl() {
  this->type = JsonNull;
  this->length = -1;
  this->key = NULL;
  this->value.stringValue = NULL;
}

JsonEl::JsonEl(bool value) : JsonEl() {
  this->type = JsonBoolean;
  this->value.booleanValue = value;
}

JsonEl::JsonEl(int value) : JsonEl() {
  this->type = JsonInt;
  this->value.intValue = value;
}

JsonEl::JsonEl(float value) : JsonEl() {
  this->type = JsonFloat;
  this->value.doubleValue = value;
}

JsonEl::JsonEl(double value) : JsonEl() {
  this->type = JsonFloat;
  this->value.doubleValue = value;
}

JsonEl::JsonEl(const char* value) : JsonEl() {
  this->type = JsonString;
  this->value.stringValue = new String(value);
}

JsonEl::JsonEl(const String& value) : JsonEl() {
  this->type = JsonString;
  this->value.stringValue = new String(value);
}

JsonEl::JsonEl(const JsonEl array[], int length) : JsonEl() {
  this->length = length;
  this->value.arrayValue = CloneArray(array, length);
  // If the elements have keys, it's an object, otherwise it's an array.
  this->type = array[0].key ? JsonObject : JsonArray;
}

JsonEl::JsonEl(const String& key, bool value) : JsonEl(value) {
  this->key = new String(key);
}

JsonEl::JsonEl(const String& key, int value) : JsonEl(value) {
  this->key = new String(key);
}

JsonEl::JsonEl(const String& key, float value) : JsonEl(value) {
  this->key = new String(key);
}

JsonEl::JsonEl(const String& key, double value) : JsonEl(value) {
  this->key = new String(key);
}

JsonEl::JsonEl(const String& key, const char* value) : JsonEl(value) {
  this->key = new String(key);
}

JsonEl::JsonEl(const String& key, const String& value) : JsonEl(value) {
  this->key = new String(key);
}

JsonEl::JsonEl(const String& key, const JsonEl array[], int length) : JsonEl(array, length) {
  this->key = new String(key);
}

JsonEl::~JsonEl() {
  if (this->key) {
    delete this->key;
  }

  switch (this->type) {
    case JsonString:
      if (this->value.stringValue) {
        delete this->value.stringValue;
      }
      break;
    case JsonArray:
    case JsonObject:
      if (this->value.arrayValue) {
        delete[] this->value.arrayValue;
      }
      break;
    default:
      break;
  }
}

void JsonEl::operator=(const JsonEl &el) {
  // Null types can be reassigned once to another type.
  if (this->type != el.type) {
    if (this->type == JsonNull) {
      this->type = el.type;
      if (this->key) {
        delete(this->key);
      }
      this->key = el.key;
      this->length = el.length;
    } else {
      LogError("Cannot assign mismatched element types");
      return;
    }
  }

  switch (this->type) {
    case JsonString:
      if (this->value.stringValue) {
        delete this->value.stringValue;
      }
      this->value.stringValue = new String(*el.value.stringValue);
      break;
    case JsonArray:
    case JsonObject:
      this->length = el.length;
      this->value.arrayValue = CloneArray(el.value.arrayValue, el.length);
      break;
    default:
      this->value = el.value;
      break;
  }
}

void JsonEl::operator=(const bool value) {
  if (this->type != JsonBoolean) {
    LogError("Cannot assign boolean to %s", this->Type());
    return;
  }
  this->value.booleanValue = value;
}

void JsonEl::operator=(const int value) {
  if (this->type != JsonInt) {
    LogError("Cannot assign int to %s", this->Type());
    return;
  }
  this->value.intValue = value;
}

void JsonEl::operator=(const float value) {
  if (this->type != JsonFloat) {
    LogError("Cannot assign float to %s", this->Type());
    return;
  }
  this->value.doubleValue = value;
}

void JsonEl::operator=(const double value) {
  if (this->type != JsonFloat) {
    LogError("Cannot assign double to %s", this->Type());
    return;
  }
  this->value.doubleValue = value;
}

void JsonEl::operator=(const char* value) {
  if (this->type != JsonString) {
    LogError("Cannot assign c-string to %s", this->Type());
    return;
  }
  if (this->value.stringValue) {
    delete this->value.stringValue;
  }
  this->value.stringValue = new String(value);
}

void JsonEl::operator=(const String& value) {
  if (this->type != JsonString) {
    LogError("Cannot assign String to %s", this->Type());
    return;
  }
  if (this->value.stringValue) {
    delete this->value.stringValue;
  }
  this->value.stringValue = new String(value);
}

JsonEl& JsonEl::operator[](const int index) {
  if (this->type != JsonArray) {
    LogError("Cannot get subelement of non array type: %s", this->Type());
    return *this;
  }
  LogError("Array index '%d' not found.", index);
  return this->value.arrayValue[index];
}

JsonEl& JsonEl::operator[](const char* key) {
  if (this->type != JsonObject) {
    LogError("Cannot get key value of non object type: %s", this->Type());
    return *this;
  }
  for (int i = 0; i < this->length; i++) {
    JsonEl& el = this->value.objectValue[i];
    if (el.Key() == key) {
      return el;
    }
  }
  LogError("Key '%s' not found.", key);
  return NotFound;
}

JsonEl& JsonEl::operator[](String& key) {
  return operator[](key.c_str());
}

const char* JsonEl::Type() {
  switch(type) {
    case JsonNull:
      return "null";
    case JsonBoolean:
      return "boolean";
    case JsonInt:
      return "int";
    case JsonFloat:
      return "float";
    case JsonString:
      return "string";
    case JsonArray:
      return "array";
    case JsonObject:
      return "object";
    default:
      return "(unknown)";
  }
}

const String& JsonEl::Key() {
  if (!key) {
    return *NoKey.key;
  }
  return *key;
}

const int JsonEl::Length() {
  return length;
}

const bool JsonEl::AsBoolean() {
  if (type == JsonBoolean) {
    return value.booleanValue;
  }
  LogError("Cannot convert %s to boolean", this->Type());
  return false;
}

const int JsonEl::AsInt() {
  if (type == JsonInt) {
    return value.intValue;
  }
  LogError("Cannot convert %s to int", this->Type());
  return -1;
}

const float JsonEl::AsFloat() {
  if (type == JsonFloat) {
    return value.doubleValue;
  }
  String msg = "Cannot convert ";
  LogError("Cannot convert %s to float", this->Type());
  return NAN;
}

const double JsonEl::AsDouble() {
  if (type == JsonFloat) {
    return value.doubleValue;
  }
  LogError("Cannot convert %s to double", this->Type());
  return NAN;
}

const String JsonEl::AsString() {
  if (type == JsonString) {
    return *value.stringValue;
  }
  LogError("Cannot convert %s to String", this->Type());
  return "";
}

void JsonEl::PrintJson(Print &out) {
  PrintJson(false, out);
}

void JsonEl::PrintJson(bool pretty, Print &out) {
  PrintJson(pretty ? 0 : -1, out);
}

void JsonEl::PrintJson(int indent, Print &out) {
  switch(type) {
    case JsonNull:
      out.print("null");
      break;
    case JsonBoolean:
      out.print(value.booleanValue ? "true" : "false");
      break;
    case JsonInt:
      out.print(value.intValue);
      break;
    case JsonFloat:
      out.print(value.doubleValue);
      break;
    case JsonString:
      out.print("\"");
      out.print(*value.stringValue);
      out.print("\"");
      break;
    case JsonArray:
      PrintArrayJson(&value.arrayValue, length, indent, out);
      break;
    case JsonObject:
      PrintObjectJson(&value.arrayValue, length, indent, out);
      break;
    default:
      out.print("(unknown)");
      break;
  }
}

int JsonEl::UpdateFromJson(const String &json) {
  Serial.print("UpdateFromJson: ");
  Serial.println(json);

  switch(type) {
    case JsonBoolean:
      return UpdateFromJsonBoolean(json);
    case JsonInt:
      return UpdateFromJsonInt(json);
    case JsonFloat:
      return UpdateFromJsonFloat(json);
    case JsonString:
      return UpdateFromJsonString(json);
    case JsonArray:
      return UpdateFromJsonArray(json);
    case JsonObject:
      return UpdateFromJsonObject(json);
    case JsonNull:
    default:
      break;
  }
  return 0;
}

int JsonEl::UpdateFromJsonBoolean(const String &json) {
  String strValue = json;
  strValue.trim();
  strValue.toLowerCase();

  if (strValue == "true") {
    this->value.booleanValue = true;
    return 1;
  }
  if (strValue == "false") {
    this->value.booleanValue = false;
    return 1;
  }
  LogError("Failed to update boolean value from json: %s", json.c_str());
  return 0;
}

int JsonEl::UpdateFromJsonInt(const String &json) {
  String strValue = json;
  strValue.trim();
  strValue.toLowerCase();

  if (!IsInt(strValue)) {
    LogError("Failed to update int value from json: %s", json.c_str());
    return 0;
  }
  this->value.intValue = strValue.toInt();
  return 1;
}

int JsonEl::UpdateFromJsonFloat(const String &json) {
  String strValue = json;
  strValue.trim();
  strValue.toLowerCase();

  if (!IsFloat(strValue)) {
    LogError("Failed to update float value from json: %s", json.c_str());
    return 0;
  }
  this->value.doubleValue = strValue.toDouble();
  return 1;
}

int JsonEl::UpdateFromJsonString(const String &json) {
  if (this->value.stringValue) {
    delete this->value.stringValue;
  }
  this->value.stringValue = new String(json);
  this->value.stringValue->trim();
  return 1;
}

int JsonEl::UpdateFromJsonArray(const String &json) {
  String remainingJson = json;
  int valueEndIndex;
  String valueStr;
  int arrayIndex = 0;
  int changeCount = 0;
  Serial.println("--- UpdateFromJsonArray");
  Serial.print("--- json=");
  Serial.println(json);
  remainingJson.trim();

/*
  if (remainingJson[0] != '[' || remainingJson[remainingJson.length()-1] != ']') {
    LogError("Failed to update array value from json: %s", json.c_str());
    return 0;
  }
  remainingJson = remainingJson.substring(1, remainingJson.length() - 1);

  while(remainingJson.length() > 0) {
    valueEndIndex = FindChunkEnd(remainingJson, 0, ',');
    if (valueEndIndex == -1) {
      return changeCount;
    }
    valueStr = remainingJson.substring(0, valueEndIndex);
    valueStr.trim();

    changeCount += (*this)[arrayIndex].UpdateFromJson(valueStr);
    remainingJson = remainingJson.substring(valueEndIndex + 1);
    arrayIndex++;
  }
*/
  return changeCount;
}

int JsonEl::UpdateFromJsonObject(const String &json) {
  int startIndex = json.indexOf('{');
  int endIndex = json.lastIndexOf('}');
  int index = startIndex + 1;
  String valueStr;
  int changeCount = 0;
  Serial.println("--- UpdateFromJsonObject");
  Serial.print("--- json=");
  Serial.println(json);

  if (startIndex == -1 || endIndex == -1 || endIndex < startIndex) {
    LogError("Failed to update object value from json: %s", json.c_str());
    return 0;
  }

  while (index < endIndex) {
    int colonIndex = json.indexOf(':', index);
    Serial.print("--- colonIndex=");
    Serial.println(colonIndex);
    String key = StripQuotes(json, index, colonIndex);
    Serial.print("--- key=");
    Serial.println(key);
    int valueEndIndex = FindChunkEnd(json, colonIndex + 1, ',');
    Serial.print("--- valueEndIndex=");
    Serial.println(valueEndIndex);

    if (valueEndIndex == -1) {
      return changeCount;
    }
    valueStr = json.substring(colonIndex + 1, valueEndIndex);
    valueStr.trim();
    Serial.print("--- valueStr=");
    Serial.println(valueStr);

    //changeCount += (*this)[key].UpdateFromJson(valueStr);
    index = valueEndIndex + 1;
  }

  return changeCount;
}

JsonEl* JsonEl::CloneArray(const JsonEl* array, int length) {
  JsonEl* newArray = new JsonEl[length];

  for (int i = 0; i < length; i++) {
    newArray[i] = array[i];
  }
  return newArray;
}

void JsonEl::PrintArrayJson(JsonEl* array[], int length, int indent, Print &out) {
  int childIndent = indent >= 0 ? indent + 1 : -1;

  out.print("[");
  for (int i = 0; i < length; i++) {
    JsonEl& element = (*array)[i];
    IndentNewline(childIndent, out);
    element.PrintJson(childIndent, out);
    if (i < length - 1) {
      out.print(',');
    }
  }
  IndentNewline(indent, out);
  out.print("]");
}

void JsonEl::PrintObjectJson(JsonEl* array[], int length, int indent, Print &out) {
  int childIndent = indent >= 0 ? indent + 1 : -1;

  out.print("{");
  for (int i = 0; i < length; i++) {
    JsonEl& element = (*array)[i];
    IndentNewline(childIndent, out);
    out.print("\"");
    out.print(*(element.key));
    out.print("\":");
    if (indent >= 0) {
      out.print(" ");
    }
    element.PrintJson(childIndent, out);
    if (i < length - 1) {
      out.print(',');
    }
  }
  IndentNewline(indent, out);
  out.print("}");
}

void JsonEl::IndentNewline(int times, Print &out) {
  if (times < 0) {
    return;
  }

  out.println();

  for (int i = 0; i < times; i++) {
    out.print("  ");
  }
}

bool JsonEl::IsInt(const String& str) {
  bool isExponential = false;

  for (int i = 0; i < str.length(); i++) {
    int ch = str[i];
    if (isDigit(ch)) {
      continue;
    }
    if (ch == '-' && i == 0) {
      continue;
    }
    // None of the above are true, so this is not a JSON-compatible number.
    return false;
  }
  return true;
}

bool JsonEl::IsFloat(const String& str) {
  bool isDecimal = false;
  bool isExponential = false;

  for (int i = 0; i < str.length(); i++) {
    int ch = str[i];
    if (isDigit(ch)) {
      continue;
    }
    if (ch == '-' && i == 0) {
      continue;
    }
    if (ch == '.' && !isDecimal) {
      isDecimal = true;
      continue;
    }
    if (ch == 'e' && !isExponential && isDecimal) {
      isExponential = true;
      continue;
    }
    // None of the above are true, so this is not a JSON-compatible number.
    return false;
  }
  return true;
}

String JsonEl::StripQuotes(const String& str, int begin, int end) {
  // Advance past whitespace
  while (str[begin] == ' ' || str[begin] == '\t' && begin < end) {
    begin++;
  }
  while (str[end] == ' ' || str[end] == '\t' && begin < end) {
    end--;
  }

  // Remove outermost matching quotes
  if ((str[begin] == '"' && str[end] == '"') ||
      (str[begin] == '\'' && str[end] == '\'')) {
    begin++;
    end--;
  }

  return str.substring(begin, end);
}

int JsonEl::FindChunkEnd(const String& str, int begin, char endChar) {
  Serial.println("---- FindChunkEnd");
  Serial.print("---- endChar=");
  Serial.println(endChar);
  for (int i = begin; i < str.length(); i++) {
    Serial.print("---- i=");
    Serial.print(i);
    Serial.print(" str[i]=");
    Serial.println(str[i]);
    if (str[i] == '\\') {
      Serial.println("---- escape");
      // Escape next character
      i++;
    } else if (str[i] == endChar) {
      Serial.println("---- found endChar");
      // We found the end char, and it's not contained in a subelement or string.
      return i;
    } else if (str[i] == '\'') {
      Serial.println("---- begin single quote string");
      // It's a string, advance past the end of it.
      //i = FindChunkEnd(str, i + 1, '\'');
    } else if (str[i] == '"') {
      Serial.println("---- begin double quote string");
      // It's a string, advance past the end of it.
      //i = FindChunkEnd(str, i + 1, '"');
    } else if (str[i] == '{') {
      Serial.println("---- begin curly brace");
      // It's an object, advance past the end of it.
      //i = FindChunkEnd(str, i + 1, '}');
/*
    } else if (str[i] == '[') {
      Serial.println("---- begin square bracket");
      // It's an array, advance past the end of it.
      //i = FindChunkEnd(str, i + 1, ']');
*/
    }
  }
  Serial.println("---- reached end of string");
/*
  if (endChar == ',') {
    Serial.println("---- endchar is comma, return string");
    return str.length();
  }

  Serial.println("---- end of function. error:");
  LogError("Failed to find text chunk ending in '%c' in text: '%s'", endChar, str.substring(begin).c_str());
*/
  return -1;
}
