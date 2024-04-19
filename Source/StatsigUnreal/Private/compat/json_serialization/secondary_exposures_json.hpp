#pragma once

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Templates/SharedPointer.h"

#include "unreal_json_utils.hpp"

#include <string>

namespace statsig::data_types::secondary_exposures {

inline TArray<TSharedPtr<FJsonValue>> ToJson(
    const std::vector<data::SecondaryExposure>&
    secondary_exposures
    ) {
  TArray<TSharedPtr<FJsonValue>> sec_expo_json_array;

  for (const auto& sec_expo : secondary_exposures) {
    TSharedPtr<FJsonObject> sec_expo_json = MakeShared<FJsonObject>();
    for (const auto& pair : sec_expo) {
      const FString Key = TO_FSTRING(pair.first);
      const FString Value = TO_FSTRING(pair.second);
      sec_expo_json->SetStringField(Key, Value);
    }
    sec_expo_json_array.Add(MakeShared<FJsonValueObject>(sec_expo_json));
  }

  return sec_expo_json_array;
}

inline std::vector<data::SecondaryExposure> FromJson(
    TSharedPtr<FJsonObject> json, FString field) {
  std::vector<data::SecondaryExposure> exposures;

  const TArray<TSharedPtr<FJsonValue>>* arr;
  if (!json->TryGetArrayField(field, arr)) {
    return exposures;
  }

  for (const TSharedPtr<FJsonValue>& value : *arr) {
    if (value.IsValid() && value->Type == EJson::Object) {
      TSharedPtr<FJsonObject> object = value->AsObject();
      exposures.push_back(unreal_json_utils::JsonObjectToUnorderedStringMap(object));
    }
  }

  return exposures;
}

}
