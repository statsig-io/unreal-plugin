#pragma once

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

#include <optional>
#include <string>

namespace statsig {

typedef TSharedPtr<FJsonObject> JsonObject;
typedef TSharedPtr<FJsonValue> JsonValue;

inline JsonObject EmptyJsonObject() {
  return MakeShareable(new FJsonObject());
}

inline JsonValue GetJsonValueFromJsonObject(
    const std::string key,
    const JsonObject& object) {
  return object.IsValid()
           ? object->Values[FString(key.c_str())]
           : MakeShareable(new FJsonValueNull());
}

inline JsonObject GetSafeJsonObject(const JsonObject& object) {
  return object.IsValid() ? object : MakeShareable(new FJsonObject());
}

inline std::unordered_map<std::string, JsonValue> StringMapToJsonValueMap(
    const std::unordered_map<std::string, std::string>& map) {
  std::unordered_map<std::string, JsonValue> result;
  for (const auto& [fst, snd] : map) {
    const auto value = FString(snd.c_str());
    result[fst] = MakeShareable(new FJsonValueString(value));
  }

  return result;
}

inline void AddToJsonObject(const JsonObject& obj, const std::string key,
                            const JsonValue value) {
  obj->SetField(FString(key.c_str()), value);
}

inline String JsonValueToCompatString(const JsonValue& input) {
  FString json_str;
  FJsonSerializer::Serialize(
      input.ToSharedRef(),
      "",
      TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(
          &json_str)
      );

  return json_str;
}

inline JsonValue JsonValueFromNumber(const long& input) {
  const auto str = FString(std::to_string(input).c_str());
  return MakeShareable(new FJsonValueString(str));
}

inline JsonValue TimeToJsonValue(const time_t& input) {
  return MakeShareable(new FJsonValueNumber(input));
}

inline JsonValue IntToJsonValue(const int& input) {
  return MakeShareable(new FJsonValueNumber(input));
}

inline JsonValue BoolToJsonValue(const bool input) {
  return MakeShareable(new FJsonValueBoolean(input));
}

inline JsonValue CompatStringToJsonValue(const String& input) {
  return MakeShareable(new FJsonValueString(input));
}

inline JsonValue StringToJsonValue(const std::string& input) {
  return MakeShareable(new FJsonValueString(FString(input.c_str())));
}

inline JsonValue JsonObjectToJsonValue(const JsonObject& input) {
  return MakeShared<FJsonValueObject>(input);
}

inline JsonValue JsonValueMapToJsonValue(
    const std::unordered_map<std::string, JsonValue>& input) {
  const TSharedPtr<FJsonObject> result = MakeShareable(new FJsonObject());

  for (const auto& [fst, snd] : input) {
    AddToJsonObject(result, fst, snd);
  }

  return JsonObjectToJsonValue(result);
}


inline JsonValue JsonArrayToJsonValue(const std::vector<JsonValue>& input) {
  TArray<TSharedPtr<FJsonValue>> events_json_arr;
  for (const auto& value : input) {
    events_json_arr.Add(value);
  }
  return MakeShared<FJsonValueArray>(events_json_arr);
}

}
