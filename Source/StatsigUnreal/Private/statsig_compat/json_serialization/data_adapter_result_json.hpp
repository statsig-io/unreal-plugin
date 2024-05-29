#pragma once

#include <string>

#include "evaluations_data_adapter.h"
#include "statsig_compat/json_serialization/unreal_json_utils.hpp"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Templates/SharedPointer.h"

namespace statsig::data_types::data_adapter_result {

inline StatsigResult<std::string> Serialize(const DataAdapterResult& result) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("data"), FString(result.data.c_str()));
  json->SetNumberField(TEXT("source"), static_cast<int>(result.source));
  json->SetNumberField(TEXT("receivedAt"), result.received_at);
  json->SetStringField(
      TEXT("fullUserHash"), FString(result.full_user_hash.c_str()));

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

  const time_t received_at = json->GetNumberField(TEXT("receivedAt"));
  DataAdapterResult result{
      TCHAR_TO_UTF8(*json->GetStringField(TEXT("fullUserHash"))),
      received_at
  };
  result.data = TCHAR_TO_UTF8(*json->GetStringField(TEXT("data")));
  result.source =
      static_cast<ValueSource>(json->GetNumberField(TEXT("source")));

  return {Ok, result};
}

} // namespace statsig::data_types::data_adapter_result
