#pragma once

#include <optional>
#include <string>


namespace statsig {

typedef TSharedPtr<FJsonObject> JsonObject;
typedef TSharedPtr<FJsonValue> JsonValue;

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

}
