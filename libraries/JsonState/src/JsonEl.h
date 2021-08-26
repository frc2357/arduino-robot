#ifndef JSONEL_H
#define JSONEL_H

#include <Arduino.h>

class JsonEl {
  public:
    static JsonEl NotFound;
    static JsonEl NoKey;

    static Print& GetErrorLogger();
    static void SetErrorLogger(Print& log);

    JsonEl();
    JsonEl(bool value);
    JsonEl(int value);
    JsonEl(float value);
    JsonEl(double value);
    JsonEl(const char* value);
    JsonEl(const String& value);
    JsonEl(const JsonEl array[], int length);

    JsonEl(const String& key, bool value);
    JsonEl(const String& key, int value);
    JsonEl(const String& key, float value);
    JsonEl(const String& key, double value);
    JsonEl(const String& key, const char* value);
    JsonEl(const String& key, const String& value);
    JsonEl(const String& key, const JsonEl array[], int length);

    ~JsonEl();

    void operator=(const JsonEl& el);
    void operator=(const bool value);
    void operator=(const int value);
    void operator=(const float value);
    void operator=(const double value);
    void operator=(const char* value);
    void operator=(const String& value);

    JsonEl& operator[](const int index);
    JsonEl& operator[](const char* key);
    JsonEl& operator[](String& key);

    const char* Type();
    const String& Key();
    const int Length();

    const bool AsBoolean();
    const int AsInt();
    const float AsFloat();
    const double AsDouble();
    const String AsString();

    void PrintJson(Print& out);
    void PrintJson(bool pretty, Print& out);

    int UpdateFromJson(const String& json);

  private:
    enum JsonElementType { JsonNull, JsonBoolean, JsonInt, JsonFloat, JsonString, JsonArray, JsonObject };
    static Print* errorLog;

    static void LogError(const char *format, ...);

    union JsonElementValue {
      bool booleanValue;
      int intValue;
      double doubleValue;
      String* stringValue;
      JsonEl* arrayValue;
      JsonEl* objectValue;
    };

    String *key;
    JsonElementType type;
    JsonElementValue value;
    int length;

    void PrintJson(int indent, Print& out);
    int UpdateFromJsonBoolean(const String& json);
    int UpdateFromJsonInt(const String& json);
    int UpdateFromJsonFloat(const String& json);
    int UpdateFromJsonString(const String& json);
    int UpdateFromJsonArray(const String& json);
    int UpdateFromJsonObject(const String& json);

    static JsonEl* CloneArray(const JsonEl* array, int length);
    static void PrintArrayJson(JsonEl *array[], int length, int indent, Print& out);
    static void PrintObjectJson(JsonEl *array[], int length, int indent, Print& out);
    static void IndentNewline(int times, Print& out);
    static boolean StringToBoolean(const String& value);
    static int StringToInt(const String& value);
    static double StringToDouble(const String& value);
    static JsonEl* StringToArray(const String& value);
    static JsonEl* StringToObject(const String& value);
    static bool IsInt(const String& str);
    static bool IsFloat(const String& str);
    static String StripQuotes(const String& str, int begin, int end);
    static int FindChunkEnd(const String& str, int begin, char endChar);
};

#endif /* JSONEL_H */
