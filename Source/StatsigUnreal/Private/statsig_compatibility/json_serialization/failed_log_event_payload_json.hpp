#pragma once

#include "statsig_event_internal.hpp"
#include "statsig_event_json.hpp"

namespace statsig::data_types::failed_log_event_payload {

inline std::optional<internal::FailedEventPayload> FromJson(
    const TSharedPtr<FJsonObject>& json) {
  const TArray<TSharedPtr<FJsonValue>>* events_json_arr;
  if (!json->TryGetArrayField("events", events_json_arr)) {
    return std::nullopt;
  }

  internal::FailedEventPayload failure;

  for (const TSharedPtr<FJsonValue>& value : *events_json_arr) {
    if (value.IsValid() && value->Type == EJson::Object) {
      TSharedPtr<FJsonObject> object = value->AsObject();
      failure.events.push_back(statsig_event::FromJson(object));
    }
  }

  if (failure.events.size() == 0) {
    return std::nullopt;
  }

  failure.attempts = json->GetNumberField("attempts");

  return failure;
}

inline std::string Serialize(
    const std::vector<internal::FailedEventPayload>& failures) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  TArray<TSharedPtr<FJsonValue>> failures_json_arr;

  for (const auto& failure : failures) {
    TSharedPtr<FJsonObject> failure_json = MakeShared<FJsonObject>();

    failure_json->SetNumberField("attempts", failure.attempts);
    TArray<TSharedPtr<FJsonValue>> events_json;
    for (internal::StatsigEventInternal event : failure.events) {
      auto event_json = MakeShared<FJsonValueObject>(statsig_event::ToJson(event));
      events_json.Add(event_json);
    }
    failure_json->SetArrayField("events", events_json);

    failures_json_arr.Add(MakeShared<FJsonValueObject>(failure_json));
  }

  return unreal_json_utils::JsonArrayToString(failures_json_arr);
}

inline std::optional<std::vector<internal::FailedEventPayload>> Deserialize(
    const std::string& input) {
  TSharedPtr<FJsonValue> json_arr;
  const TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<UTF32CHAR>::Create(
      TO_FSTRING(input));

  if (!FJsonSerializer::Deserialize(reader, json_arr)) {
    return std::nullopt;
  }

  if (json_arr == nullptr || !json_arr.IsValid() || json_arr->Type !=
      EJson::Array) {
    return std::nullopt;
  }

  std::vector<internal::FailedEventPayload> failures;
  for (const TSharedPtr<FJsonValue>& json : json_arr->AsArray()) {
    if (json->Type != EJson::Object) {
      continue;
    }

    const auto failure = FromJson(json->AsObject());
    if (failure.has_value()) {
      failures.push_back(failure.value());
    }
  }

  if (failures.size() == 0) {
    return std::nullopt;
  }

  return failures;
}

}
