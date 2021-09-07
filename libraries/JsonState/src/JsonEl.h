#ifndef JSONEL_H
#define JSONEL_H

#include <Arduino.h>

class JsonElement {
  public:
    static JsonElement NotFound;
    static JsonElement NoKey;

    JsonElement();
    JsonElement(const JsonElement &element);
    ~JsonElement();

    JsonElement &operator[](const size_t index) const;
    JsonElement &operator[](const char *key) const;
    void operator=(JsonElement &element);
    void operator=(bool value);
    void operator=(int value);
    void operator=(float value);
    void operator=(double value);
    void operator=(const char *value);

    const char *type() const;
    const char *key() const;
    size_t length() const;

    bool asBoolean() const;
    int asInt() const;
    float asFloat() const;
    double asDouble() const;
    const char *asString() const;

    JsonElement &findByKey(const char *key, size_t length) const;

    void printJson(int indent, Print &out) const;

  protected:
    enum JsonElementType { NoType, BooleanType, IntType, FloatType, StringType, ArrayType, ObjectType };
    static Print &m_errorLog;

    static void logError(const char *format, ...);

    union JsonElementValue {
      bool booleanValue;
      int intValue;
      double doubleValue;
      char *stringValue;
      JsonElement *arrayValue;
    };

    const char *m_key;
    JsonElementType m_type;
    JsonElementValue m_value;
    size_t m_length;

    friend class Json;
    friend class JsonState;
    friend class JsonUtils;

    JsonElement(const char *key, JsonElementType type, JsonElementValue value, size_t length);
    void printJsonBoolean(int indent, Print &out) const;
    void printJsonInt(int indent, Print &out) const;
    void printJsonFloat(int indent, Print &out) const;
    void printJsonString(int indent, Print &out) const;
    void printJsonArray(int indent, Print &out) const;
    void printJsonObject(int indent, Print &out) const;

    size_t updateFromJson(const char *json, size_t length, size_t &elementsUpdated);
    size_t updateFromJsonBoolean(const char *json, size_t length, size_t &elementsUpdated);
    size_t updateFromJsonInt(const char *json, size_t length, size_t &elementsUpdated);
    size_t updateFromJsonFloat(const char *json, size_t length, size_t &elementsUpdated);
    size_t updateFromJsonString(const char *json, size_t length, size_t &elementsUpdated);
    size_t updateFromJsonArray(const char *json, size_t length, size_t &elementsUpdated);
    size_t updateFromJsonObject(const char *json, size_t length, size_t &elementsUpdated);
};

class Json {
  public:
    static JsonElement Boolean(bool value);
    static JsonElement Boolean(const char *key, bool value);

    static JsonElement Int(int value);
    static JsonElement Int(const char *key, int value);

    static JsonElement Float(float value);
    static JsonElement Float(double value);
    static JsonElement Float(const char *key, float value);
    static JsonElement Float(const char *key, double value);

    static JsonElement String(const char *value);
    static JsonElement String(const char *value, size_t length);
    static JsonElement String(const char *key, const char *value);
    static JsonElement String(const char *key, const char *value, size_t length);

    template<size_t S>
    static JsonElement Array(JsonElement (&array)[S]);
    template<size_t S>
    static JsonElement Array(const char *key, JsonElement (&array)[S]);

    template<size_t S>
    static JsonElement Object(JsonElement (&fields)[S]);
    template<size_t S>
    static JsonElement Object(const char *key, JsonElement (&fields)[S]);
};

class JsonState {
  public:
    JsonState(JsonElement &root);
    JsonState(JsonElement &root, Print &errorLogger);
    ~JsonState();

    void printJson(Print &out) const;
    void printJson(bool pretty, Print &out) const;
    bool updateFromJson(const char *json);
    size_t updateFromJson(const char *json, size_t length, size_t &elementsUpdated);

  private:
    Print &m_errorLog;
    JsonElement &m_root;
};

class JsonUtils {
  public:
    static void indentNewline(int times, Print &out);
    static bool isWhitespace(char ch);
    static size_t countWhitespace(const char* str, size_t length);
    static size_t copyJsonString(char *dest, const char *src, size_t length);
    static size_t getIntStringLength(const char* str, size_t length);
    static size_t getFloatStringLength(const char* str, size_t length);
    static size_t getQuoteStringLength(const char* str, size_t length);
    static size_t getObjectKeyStringLength(const char* str, size_t length);
    static JsonElement &findElementByJsonKey(JsonElement &jsonObject, const char* str, size_t length);
};

#endif /* JSONEL_H */
