#pragma once

#define TO_FSTRING(input) FString(input.c_str())
#define FROM_FSTRING(input) TCHAR_TO_UTF8(*input)

namespace statsig::data_types::unreal_json_utils {

inline std::string JsonObjectToString(const TSharedPtr<FJsonObject>& json) {
  FString json_str;
  FJsonSerializer::Serialize(
      json.ToSharedRef(),
      TJsonWriterFactory<>::Create(&json_str)
      );

  return FROM_FSTRING(json_str);
}

inline TSharedPtr<FJsonObject> StringToJsonObject(const std::string &input) {
  TSharedPtr<FJsonObject> json;
  const TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(TO_FSTRING(input));

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

inline std::unordered_map<std::string, std::string> JsonObjectToUnorderedMap(
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


}
