#pragma once

#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"

#include "secondary_exposures_json.hpp"
#include "statsig_event_internal.hpp"
#include "statsig_user_json.hpp"

#include <optional>
#include <string>

namespace statsig::data_types::statsig_event {

using StatsigEventInternal = internal::StatsigEventInternal;

inline TSharedPtr<FJsonObject> ToJson(const StatsigEventInternal &event) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());
  json->SetStringField(TEXT("eventName"), ToCompat(event.event_name));
  json->SetNumberField(TEXT("time"), event.time);
  json->SetObjectField(TEXT("user"), statsig_user::ToJson(event.user));

  if (event.metadata.has_value()) {
    const TSharedPtr<FJsonObject> metadata_json =
        MakeShareable(new FJsonObject);
    for (const auto &[fst, snd] : event.metadata.value()) {
      metadata_json->SetStringField(ToCompat(fst), ToCompat(snd));
    }
    json->SetObjectField(TEXT("metadata"), metadata_json);
  }

  if (event.secondary_exposures.has_value()) {
      const auto sec_expo_json_arr = secondary_exposures::ToJson(
        event.secondary_exposures.value());
    json->SetArrayField(TEXT("secondaryExposures"), sec_expo_json_arr);
  }

  if (event.string_value.has_value()) {
    json->SetStringField(
        TEXT("value"), ToCompat(event.string_value.value()));
  } else if (event.double_value.has_value()) {
    json->SetNumberField(TEXT("value"), event.double_value.value());
  }

  return json;
}

inline StatsigEventInternal FromJson(const TSharedPtr<FJsonObject> &json) {
  StatsigEventInternal event;

  event.event_name = FromCompat(json->GetStringField(TEXT("eventName")));
  event.time = json->GetNumberField(TEXT("time"));

  FString str_value;
  double num_value;
  if (json->TryGetStringField(TEXT("value"), str_value)) {
    event.string_value = FromCompat(str_value);
  } else if (json->TryGetNumberField(TEXT("value"), num_value)) {
    event.double_value = num_value;
  }

  if (json->HasField(TEXT("user"))) {
    event.user = statsig_user::FromJson(json->GetObjectField(TEXT("user"))).value_or(
        event.user);
  }

  if (json->HasField(TEXT("metadata"))) {
    event.metadata = unreal_json_utils::JsonObjectToUnorderedStringMap(
        json->GetObjectField(TEXT("metadata")));
  }

  if (json->HasField(TEXT("secondaryExposures"))) {
    event.secondary_exposures = secondary_exposures::FromJson(
        json, TEXT("secondaryExposures"));
  }

  return event;
}

inline std::string Serialize(const StatsigEventInternal &event) {
  return unreal_json_utils::JsonObjectToString(ToJson(event));
}

inline std::optional<StatsigEventInternal>
Deserialize(const std::string &input) {
  const auto json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr || !json.IsValid()) {
    return std::nullopt;
  }

  return FromJson(json);
}

}
