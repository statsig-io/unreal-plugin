#pragma once

#include "statsig/evaluations_data_adapter.h"

namespace statsig::data_types::data_adapter_result {

inline std::string Serialize(const DataAdapterResult& result) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("data"), FString(result.data.c_str()));
  json->SetNumberField(TEXT("source"), static_cast<int>(result.source));
  json->SetNumberField(TEXT("receivedAt"), result.receivedAt);

  FString json_str;
  FJsonSerializer::Serialize(
      json.ToSharedRef(),
      TJsonWriterFactory<>::Create(&json_str)
      );

  return TCHAR_TO_UTF8(*json_str);
}

inline DataAdapterResult Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json;
  const TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(
      FString(input.c_str()));

  if (!FJsonSerializer::Deserialize(reader, json) || !json.IsValid()) {
    return {};
  }

  DataAdapterResult result;
  result.data = TCHAR_TO_UTF8(*json->GetStringField(TEXT("data")));
  result.source = static_cast<ValueSource>(json->
    GetNumberField(TEXT("source")));
  result.receivedAt = json->GetNumberField(TEXT("receivedAt"));
  return result;
}

}
