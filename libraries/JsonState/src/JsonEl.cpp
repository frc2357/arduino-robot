#include "JsonEl.h"

// ---------
// JsonState
// ---------
JsonState::JsonState(JsonElement& root) : JsonState(root, Serial) {}

JsonState::JsonState(JsonElement& root, Print& errorLog) : m_root(root), m_errorLog(errorLog) {
  JsonElement::m_errorLog = m_errorLog;
}

JsonState::~JsonState() {
  m_errorLog.println("[WARNING: JsonState must be created in the root level sketch (.ino) file!]");
}

void JsonState::printJson(Print &out) const {
  printJson(false, out);
}

void JsonState::printJson(bool pretty, Print &out) const {
  m_root.printJson(pretty ? 0 : -1, out);
}

bool JsonState::updateFromJson(const char* json) {
  size_t elementsUpdated = 0;
  size_t length = strlen(json);
  size_t charsRead = updateFromJson(json, length, elementsUpdated);

  if (charsRead == -1) {
    return false;
  }
  return true;
}

size_t JsonState::updateFromJson(const char* json, size_t length, size_t &elementsUpdated) {
  size_t whitespaceLength = JsonUtils::countWhitespace(json, length);
  json += whitespaceLength;
  length -= whitespaceLength;

  return m_root.updateFromJson(json, strlen(json), elementsUpdated);
}

// -----------
// JsonElement
// -----------
JsonElement JsonElement::NotFound;
JsonElement JsonElement::NoKey;

Print &JsonElement::m_errorLog = Serial;

void JsonElement::logError(const char *format, ...) {
  va_list args;
  char message[64];
  va_start(args, format);
  vsprintf(message, format, args);
  va_end(args);

  m_errorLog.print("[JsonState ERROR: ");
  m_errorLog.print(message);
  m_errorLog.println("]");
}

JsonElement::JsonElement() {
  m_key = "\0";
  m_type = NoType;
  m_value.arrayValue = NULL;
  m_length = -1;
}

JsonElement::JsonElement(const char *key, JsonElementType type, JsonElementValue value, size_t length) {
  m_key = key;
  m_type = type;
  m_value = value;
  m_length = length;
}

JsonElement::JsonElement(const JsonElement& element) {
  m_key = element.m_key;
  m_type = element.m_type;
  m_value = element.m_value;
  m_length = element.m_length;
}

JsonElement::~JsonElement() {
  //logError("Json Elements should only be declared in the root .ino sketch file!");
}

JsonElement& JsonElement::operator[](const size_t index) const {
  if (m_type != ArrayType && m_type != ObjectType) {
    logError("Cannot access subelement of type %s", type());
    return NotFound;
  }

  if (index > m_length) {
    logError("Array index '%d' out of bounds.", index);
  }
  return m_value.arrayValue[index];
}

JsonElement& JsonElement::operator[](const char* key) const {
  return findByKey(key, strlen(key));
}

void JsonElement::operator=(JsonElement& element) {
  m_key = element.m_key;
  m_type = element.m_type;
  m_value = element.m_value;
  m_length = element.m_length;
}

void JsonElement::operator=(bool value) {
  if (m_type == BooleanType) {
    m_value.booleanValue = value;
  } else {
    logError("Cannot assign boolean value to type %s", type());
  }
}

void JsonElement::operator=(int value) {
  if (m_type == IntType) {
    m_value.intValue = value;
  } else if (m_type == FloatType) {
    m_value.doubleValue = value;
  } else {
    logError("Cannot assign int value to type %s", type());
  }
}

void JsonElement::operator=(float value) {
  if (m_type == FloatType) {
    m_value.doubleValue = value;
  } else {
    logError("Cannot assign float value to type %s", type());
  }
}

void JsonElement::operator=(double value) {
  if (m_type == FloatType) {
    m_value.doubleValue = value;
  } else {
    logError("Cannot assign double value to type %s", type());
  }
}

void JsonElement::operator=(const char *value) {
  if (m_type == StringType) {
    if (m_length < strlen(value) + 1) {
      logError("Concatenating incoming string: %s to %d chars", value, m_length - 1);
    }
    strlcpy(m_value.stringValue, value, m_length);
  } else {
    logError("Cannot assign string value to type %s", type());
  }
}

const char* JsonElement::type() const {
  switch(m_type) {
    case BooleanType:
      return "boolean";
    case IntType:
      return "int";
    case FloatType:
      return "float";
    case StringType:
      return "string";
    case ArrayType:
      return "array";
    case ObjectType:
      return "object";
    default:
      return "(unknown)";
  }
}

const char* JsonElement::key() const {
  if (m_key[0] == '\0') {
    return NoKey.m_key;
  }
  return m_key;
}

size_t JsonElement::length() const {
  return m_length;
}

bool JsonElement::asBoolean() const {
  if (m_type == BooleanType) {
    return m_value.booleanValue;
  }
  logError("Cannot access type %s as boolean", type());
  return false;
}

int JsonElement::asInt() const {
  if (m_type == IntType) {
    return m_value.intValue;
  }
  logError("Cannot access type %s as int", type());
  return -1;
}

float JsonElement::asFloat() const {
  if (m_type == FloatType) {
    return (float) m_value.doubleValue;
  }
  logError("Cannot access type %s as float", type());
  return NAN;
}

double JsonElement::asDouble() const {
  if (m_type == FloatType) {
    return m_value.doubleValue;
  }
  logError("Cannot access type %s as double", type());
  return NAN;
}

const char* JsonElement::asString() const {
  if (m_type == StringType) {
    return m_value.stringValue;
  }
  logError("Cannot access type %s as string", type());
  return "";
}

JsonElement &JsonElement::findByKey(const char *key, size_t length) const {
  if (m_type != ObjectType) {
    logError("Cannot access key of type %s", type());
    return NotFound;
  }

  for (size_t i = 0; i < m_length; i++) {
    JsonElement& el = m_value.arrayValue[i];
    if (strncmp(el.key(), key, length) == 0) {
      return el;
    }
  }
  logError("Key '%s' not found.", key);
  return NotFound;
}

void JsonElement::printJson(int indent, Print& out) const {
  switch (m_type) {
    case BooleanType:
      return printJsonBoolean(indent, out);
    case IntType:
      return printJsonInt(indent, out);
    case FloatType:
      return printJsonFloat(indent, out);
    case StringType:
      return printJsonString(indent, out);
    case ArrayType:
      return printJsonArray(indent, out);
    case ObjectType:
      return printJsonObject(indent, out);
    default:
      out.print("\"unknown type\"");
  }
}

void JsonElement::printJsonBoolean(int indent, Print& out) const {
  out.print(m_value.booleanValue ? "true" : "false");
}

void JsonElement::printJsonInt(int indent, Print& out) const {
  out.print(m_value.intValue);
}

void JsonElement::printJsonFloat(int indent, Print& out) const {
  out.print(m_value.doubleValue);
}

void JsonElement::printJsonString(int indent, Print& out) const {
  out.print("\"");
  out.print(m_value.stringValue);
  out.print("\"");
}

void JsonElement::printJsonArray(int indent, Print& out) const {
  int childIndent = indent >= 0 ? indent + 1 : -1;

  out.print("[");
  for (size_t i = 0; i < m_length; i++) {
    JsonElement& element = m_value.arrayValue[i];
    JsonUtils::indentNewline(childIndent, out);
    element.printJson(childIndent, out);
    if (i < m_length - 1) {
      out.print(',');
    }
  }
  JsonUtils::indentNewline(indent, out);
  out.print("]");
}

void JsonElement::printJsonObject(int indent, Print& out) const {
  int childIndent = indent >= 0 ? indent + 1 : -1;

  out.print("{");
  for (size_t i = 0; i < m_length; i++) {
    JsonElement& element = m_value.arrayValue[i];
    JsonUtils::indentNewline(childIndent, out);
    out.print("\"");
    out.print(element.key());
    out.print("\":");
    if (indent >= 0) {
      out.print(" ");
    }
    element.printJson(childIndent, out);
    if (i < m_length - 1) {
      out.print(',');
    }
  }
  JsonUtils::indentNewline(indent, out);
  out.print("}");
}

size_t JsonElement::updateFromJson(const char* json, size_t length, size_t &elementsUpdated) {
  switch(m_type) {
    case BooleanType:
      return updateFromJsonBoolean(json, length, elementsUpdated);
    case IntType:
      return updateFromJsonInt(json, length, elementsUpdated);
    case FloatType:
      return updateFromJsonFloat(json, length, elementsUpdated);
    case StringType:
      return updateFromJsonString(json, length, elementsUpdated);
    case ArrayType:
      return updateFromJsonArray(json, length, elementsUpdated);
    case ObjectType:
      return updateFromJsonObject(json, length, elementsUpdated);
    default:
      return 0;
  }
}

size_t JsonElement::updateFromJsonBoolean(const char *json, size_t length, size_t &elementsUpdated) {
  if (strncasecmp(json, "true", 4) == 0) {
    m_value.booleanValue = true;
    return 4;
  }
  if (strncasecmp(json, "false", 5) == 0) {
    m_value.booleanValue = false;
    return 5;
  }

  logError("Expected 'true' or 'false': %s", json);
  elementsUpdated++;
  return -1;
}

size_t JsonElement::updateFromJsonInt(const char *json, size_t length, size_t &elementsUpdated) {
  size_t intLength = JsonUtils::getIntStringLength(json, length);

  if (intLength == -1) {
    return -1;
  }

  m_value.intValue = atoi(json);
  elementsUpdated++;
  return intLength;
}

size_t JsonElement::updateFromJsonFloat(const char *json, size_t length, size_t &elementsUpdated) {
  size_t floatLength = JsonUtils::getFloatStringLength(json, length);

  if (floatLength == -1) {
    logError("Expected float: %s", json);
    return -1;
  }

  m_value.doubleValue = atof(json);
  elementsUpdated++;
  return floatLength;
}

size_t JsonElement::updateFromJsonString(const char *json, size_t length, size_t &elementsUpdated) {
  size_t stringLength = JsonUtils::getQuoteStringLength(json, length);

  if (stringLength == -1) {
    logError("Expected quoted string: %s", json);
    return -1;
  }

  strlcpy(m_value.stringValue, &json[1], stringLength -1);
  elementsUpdated++;
  return stringLength;
}

size_t JsonElement::updateFromJsonArray(const char *json, size_t length, size_t &elementsUpdated) {
  if (json[0] != '[') {
    logError("Expected array to start with '[': %s", json);
    return -1;
  }

  const char *arrayJson = json + 1;
  size_t remainingLength = length - 1;
  size_t arrayIndex = 0;

  // Advance past whitespace before first value
  size_t whitespaceLength = JsonUtils::countWhitespace(arrayJson, remainingLength);
  arrayJson += whitespaceLength;
  remainingLength -= whitespaceLength;

  while (remainingLength > 0 && arrayJson[0] != '\0') {
    // Advance past whitespace before element
    size_t whitespaceLength = JsonUtils::countWhitespace(arrayJson, remainingLength);
    arrayJson += whitespaceLength;
    remainingLength -= whitespaceLength;

    // Update the element
    JsonElement &element = (*this)[arrayIndex];
    size_t elementLength = element.updateFromJson(arrayJson, remainingLength, elementsUpdated);
    if (elementLength == -1) {
      return -1;
    }
    arrayJson += elementLength;
    remainingLength -= elementLength;

    // Advance past whitespace after value
    whitespaceLength = JsonUtils::countWhitespace(arrayJson, remainingLength);
    arrayJson += whitespaceLength;
    remainingLength -= whitespaceLength;

    if (arrayJson[0] == ']') {
      // We've reached the end of this array.
      this->m_length = arrayIndex + 1;
      return (length - remainingLength) + 1;
    }
    if (arrayJson[0] == ',') {
      // Another element is next, advance past comma and iterate
      arrayIndex++;
      arrayJson++;
      remainingLength--;
      continue;
    }

    // We didn't find the end of the object or a comma, so this is not a well-formed object.
    logError("Expected either ',' or ']': ", arrayJson);
    this->m_length = arrayIndex + 1;
    return -1;
  }

  // If we didn't return from the ']' inside the loop, it's an error.
  logError("Expected array end ']': ", arrayJson);
  this->m_length = arrayIndex + 1;
  return -1;
}

size_t JsonElement::updateFromJsonObject(const char *json, size_t length, size_t &elementsUpdated) {
  if (json[0] != '{') {
    logError("Expected object to start with '{': %s", json);
    return -1;
  }

  // Advance past open { brace
  const char *objectJson = json + 1;
  size_t remainingLength = length - 1;

  // Advance past whitespace before first value
  size_t whitespaceLength = JsonUtils::countWhitespace(objectJson, remainingLength);
  objectJson += whitespaceLength;
  remainingLength -= whitespaceLength;

  while (remainingLength > 0 && objectJson[0] != '\0') {
    // Advance past whitespace before key
    size_t whitespaceLength = JsonUtils::countWhitespace(objectJson, remainingLength);
    objectJson += whitespaceLength;
    remainingLength -= whitespaceLength;

    size_t colonIndex = strcspn(objectJson, ":");
    size_t keyLength = JsonUtils::getObjectKeyStringLength(objectJson, colonIndex);
    if (keyLength == -1) {
      return -1;
    }

    JsonElement &element = JsonUtils::findElementByJsonKey(*this, objectJson, keyLength);
    if (&element == &JsonElement::NoKey) {
      logError("Failed to find element by key: %s", objectJson);
      return -1;
    }

    // Advance past key and :
    objectJson += colonIndex + 1;
    remainingLength -= colonIndex + 1;

    // Advance past whitespace before value
    whitespaceLength = JsonUtils::countWhitespace(objectJson, remainingLength);
    objectJson += whitespaceLength;
    remainingLength -= whitespaceLength;

    // Update the field element
    size_t elementLength = element.updateFromJson(objectJson, remainingLength, elementsUpdated);
    if (elementLength == -1) {
      return -1;
    }

    objectJson += elementLength;
    remainingLength -= elementLength;

    // Advance past whitespace after value
    whitespaceLength = JsonUtils::countWhitespace(objectJson, remainingLength);
    objectJson += whitespaceLength;
    remainingLength -= whitespaceLength;

    if (objectJson[0] == '}') {
      // We've reached the end of this object.
      return length - remainingLength;
    }
    if (objectJson[0] == ',') {
      // Another field is next, advance past comma and iterate
      objectJson++;
      remainingLength--;
      continue;
    }

    // We didn't find the end of the object or a comma, so this is not a well-formed object.
    logError("Expected either ',' or '}': ", objectJson);
    return -1;
  }

  // If we didn't return from the '}' inside the loop, it's an error.
  logError("Expected object end '}': ", objectJson);
  return -1;
}


// ----
// Json
// ----
JsonElement Json::Boolean(bool value) {
  return Boolean("", value);
}

JsonElement Json::Boolean(const char *key, bool value) {
  JsonElement::JsonElementValue elementValue;
  elementValue.booleanValue = value;
  return JsonElement(key, JsonElement::BooleanType, elementValue, -1);
}

JsonElement Json::Int(int value) {
  return Int("", value);
}

JsonElement Json::Int(const char *key, int value) {
  JsonElement::JsonElementValue elementValue;
  elementValue.intValue = value;
  return JsonElement(key, JsonElement::IntType, elementValue, -1);
}

JsonElement Json::Float(float value) {
  return Float("", (double)value);
}

JsonElement Json::Float(double value) {
  return Float("", value);
}

JsonElement Json::Float(const char *key, float value) {
  return Float(key, (double)value);
}

JsonElement Json::Float(const char *key, double value) {
  JsonElement::JsonElementValue elementValue;
  elementValue.doubleValue = value;
  return JsonElement(key, JsonElement::FloatType, elementValue, -1);
}

JsonElement Json::String(const char *value) {
  return String("", value);
}

JsonElement Json::String(const char *value, size_t length) {
  return String("", value, length);
}

JsonElement Json::String(const char *key, const char *value) {
  return String(key, value, strlen(value) + 1);
}

JsonElement Json::String(const char *key, const char *value, size_t length) {
  size_t stringLength = length + 1; // for null-terminated strings

  JsonElement::JsonElementValue elementValue;
  elementValue.stringValue = new char[stringLength];
  strlcpy(elementValue.stringValue, value, stringLength);
  return JsonElement(key, JsonElement::StringType, elementValue, stringLength);
}

template<size_t S>
JsonElement Json::Array(JsonElement (&array)[S]) {
  return Array("", array);
}

template<size_t S>
JsonElement Json::Array(const char *key, JsonElement (&array)[S]) {
  JsonElement::JsonElementValue elementValue;
  elementValue.arrayValue = array;
  return JsonElement(key, JsonElement::ArrayType, elementValue, S);
}

template<size_t S>
JsonElement Json::Object(JsonElement (&array)[S]) {
  return Object("", array);
}

template<size_t S>
JsonElement Json::Object(const char *key, JsonElement (&array)[S]) {
  JsonElement::JsonElementValue elementValue;
  elementValue.arrayValue = array;
  return JsonElement(key, JsonElement::ObjectType, elementValue, S);
}

void JsonUtils::indentNewline(int times, Print &out) {
  if (times < 0) {
    return;
  }

  out.println();

  for (int i = 0; i < times; i++) {
    out.print("  ");
  }
}

bool JsonUtils::isWhitespace(char ch) {
  return ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r';
}

size_t JsonUtils::countWhitespace(const char* str, size_t length) {
  size_t i = 0;

  while (isWhitespace(str[i]) && i < length) {
    i++;
  }
  return i;
}

size_t JsonUtils::copyJsonString(char *dest, const char *src, size_t length) {
  char quoteChar = 0;
  size_t srcIndex = 0; 
  size_t destIndex = 0;

  if (src[0] == '"' || src[0] == '\'') {
    quoteChar = src[0];
    srcIndex++;
  }

  for (; srcIndex < length && src[srcIndex] != '\0'; srcIndex++) {
    dest[destIndex] = src[srcIndex];
    destIndex++;
  }
  dest[destIndex] = '\0';
  return destIndex;
}

size_t JsonUtils::getIntStringLength(const char* str, size_t length) {
  int i = 0;
  bool hasDigit = false;

  if (str[0] == '-') {
    // It's a negative number, advance.
    i = 1;
  }

  for (; i < length && str[i] != '\0'; i++) {
    int ch = str[i];
    if (isDigit(ch)) {
      hasDigit = true;
      continue;
    }
    if (isWhitespace(ch) || ch == ',' || ch == ']' || ch == '}') {
      if (hasDigit) {
        // We reached the end of the number
        return i;
      }
    }
    // None of the above are true, so this is not a JSON-compatible number.
    JsonElement::logError("Expected integer: %s", str);
    return -1;
  }
  // We reached the end of the string
  return i;
}

size_t JsonUtils::getFloatStringLength(const char* str, size_t length) {
  int i = 0;
  bool hasDigit = false;
  bool isDecimal = false;
  bool isExponential = false;

  if (str[0] == '-') {
    // It's a negative number, advance.
    i = 1;
  }

  for (; i < length && str[i] != '\0'; i++) {
    int ch = str[i];
    if (isDigit(ch)) {
      hasDigit = true;
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
    if (isWhitespace(ch) || ch == ',' || ch == ']' || ch == '}') {
      if (hasDigit) {
        // We reached the end of the number
        return i;
      }
    }
    // None of the above are true, so this is not a JSON-compatible number.
    JsonElement::logError("Expected floating point number: %s", str);
    return -1;
  }
  // We reached the end of the string
  return i;
}

size_t JsonUtils::getQuoteStringLength(const char* str, size_t length) {
  char quoteChar = str[0];

  for (size_t i = 1; i < length && str[i] != '\0'; i++) {
    if (str[i] == '\\') {
      // Escape next character
      i++;
      continue;
    }
    if (str[i] == quoteChar) {
      return i + 1;
    }
  }

  JsonElement::logError("Expected quote '%c': %s", quoteChar, str);
  return -1;
}

size_t JsonUtils::getObjectKeyStringLength(const char* str, size_t length) {
  if (str[0] == '"' || str[0] == '\'') {
    return getQuoteStringLength(str, length);
  }

  for (size_t i = 0; str[i] != '\0'; i++) {
    if (str[i] == ':' || isWhitespace(str[i])) {
      return i;
    }
  }
  JsonElement::logError("Expected colon ':': %s", str);
  return -1;
}

JsonElement &JsonUtils::findElementByJsonKey(JsonElement &jsonObject, const char* str, size_t length) {
  char keyEndChar = ':';
  size_t keyLength = 0;

  if (str[0] == '"' || str[0] == '\'') {
    keyEndChar = str[0];
    str++;
  }

  while (str[keyLength] != keyEndChar) {
    if (str[keyLength] == '\0' || keyLength == length) {
      JsonElement::logError("Expected key end '%c': %s", keyEndChar, str);
      return JsonElement::NotFound;
    }
    keyLength++;
  }

  return jsonObject.findByKey(str, keyLength);
}
