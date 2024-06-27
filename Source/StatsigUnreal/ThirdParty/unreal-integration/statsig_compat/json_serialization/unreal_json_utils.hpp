#pragma once

#include <optional>
#include <string>
#include <unordered_map>

#include "statsig_compat/primitives/json_value.hpp"
#include "statsig_compat/primitives/map.hpp"

#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"
#include "Templates/SharedPointer.h"

namespace statsig::data_types::unreal_json_utils {

inline bool HasRequiredFields(const TSharedPtr<FJsonObject>& json,
                              const std::vector<std::string> fields) {
  for (auto field : fields) {
    if (!json->HasField(ToCompat(field))) {
      return false;
    }
  }

  return true;
}

inline std::string JsonObjectToString(const TSharedPtr<FJsonObject>& json) {
  FString json_str;
  FJsonSerializer::Serialize(
      json.ToSharedRef(),
      TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(
          &json_str)
      );

  return FromCompat(json_str);
}

inline std::string
JsonArrayToString(const TArray<TSharedPtr<FJsonValue>>& json) {
  FString json_str;
  FJsonSerializer::Serialize(
      json,
      TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(
          &json_str)
      );

  return FromCompat(json_str);
}

inline TSharedPtr<FJsonObject> StringToJsonObject(const std::string& input) {
  TSharedPtr<FJsonObject> json;
  const TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(
      ToCompat(input));

  if (!FJsonSerializer::Deserialize(reader, json) || !json.IsValid()) {
    return nullptr;
  }

  return json;
}


inline std::optional<std::string> TryGetString(
    const TSharedPtr<FJsonObject>& json, const FString& field) {
  if (json->HasField(field)) {
    const FString value = json->GetStringField(field);
    return TCHAR_TO_UTF8(*value);
  }

  return std::nullopt;
}

inline TSharedPtr<FJsonObject> UnorderedStringMapToJsonObject(
    const StringMap& map) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  for (const auto& [fst, snd] : map) {
    json->SetStringField(fst, snd);
  }

  return json;
}


inline std::unordered_map<std::string, std::string>
JsonObjectToUnorderedStringMap(
    const TSharedPtr<FJsonObject>& JsonObject) {
  std::unordered_map<std::string, std::string> ResultMap;

  if (JsonObject.IsValid()) {
    for (auto It = JsonObject->Values.CreateConstIterator(); It; ++It) {
      const FString Key = It.Key();
      const FString Value = JsonObject->GetStringField(Key);

      ResultMap.emplace(TCHAR_TO_UTF8(*Key), TCHAR_TO_UTF8(*Value));
    }
  }

  return ResultMap;
}

inline std::unordered_map<std::string, JsonValue>
JsonObjectToUnorderedJsonValueMap(
    const TSharedPtr<FJsonObject>& json_object) {
  std::unordered_map<std::string, JsonValue> result_map;

  if (json_object.IsValid()) {
    for (auto it = json_object->Values.CreateConstIterator(); it; ++it) {
      const FString key = it.Key();
      const FString value = json_object->GetStringField(key);

      result_map.emplace(TCHAR_TO_UTF8(*key), CompatStringToJsonValue(value));
    }
  }

  return result_map;
}


inline StringMap
JsonObjectToUnorderedCompatStringMap(
    const TSharedPtr<FJsonObject>& json_object) {
  StringMap result_map;

  if (json_object.IsValid()) {
    for (auto it = json_object->Values.CreateConstIterator(); it; ++it) {
      FString key = it.Key();
      FString value = json_object->GetStringField(key);

      result_map.Emplace(MoveTemp(key), MoveTemp(value));
    }
  }

  return result_map;
}


}
