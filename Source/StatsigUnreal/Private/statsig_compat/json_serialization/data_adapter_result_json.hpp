#pragma once

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Templates/SharedPointer.h"

#include "../evaluations_data_adapter.h"
#include "unreal_json_utils.hpp"

#include <string>

namespace statsig::data_types::data_adapter_result {

inline StatsigResult<std::string> Serialize(const DataAdapterResult& result) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("data"), FString(result.data.c_str()));
  json->SetNumberField(TEXT("source"), static_cast<int>(result.source));
  json->SetNumberField(TEXT("receivedAt"), result.receivedAt);

  return {Ok, unreal_json_utils::JsonObjectToString(json)};
}

inline StatsigResult<DataAdapterResult> Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return {JsonFailureDataAdapterResult};
  }

  if (!unreal_json_utils::HasRequiredFields(json,
                                            {"data", "source", "receivedAt"})) {
    return {JsonFailureDataAdapterResult};
  }

  DataAdapterResult result;
  result.data = TCHAR_TO_UTF8(*json->GetStringField(TEXT("data")));
  result.source =
      static_cast<ValueSource>(json->GetNumberField(TEXT("source")));
  result.receivedAt = json->GetNumberField(TEXT("receivedAt"));
  return {Ok, result};
}

}  // namespace statsig::data_types::data_adapter_result
