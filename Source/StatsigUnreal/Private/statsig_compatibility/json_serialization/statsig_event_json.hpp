#pragma once

#include "statsig_event_internal.hpp"
#include "secondary_exposures_json.hpp"

namespace statsig::data_types::statsig_event {

using StatsigEventInternal = internal::StatsigEventInternal;

inline TSharedPtr<FJsonObject> ToJson(const StatsigEventInternal& event) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());
  json->SetStringField(TEXT("eventName"), TO_FSTRING(event.event_name));
  json->SetNumberField(TEXT("time"), event.time);
  json->SetObjectField(TEXT("user"), statsig_user::ToJson(event.user));

  if (event.metadata.has_value()) {
    const TSharedPtr<FJsonObject> metadata_json = MakeShareable(new FJsonObject);
    for (const auto& [fst, snd] : event.metadata.value()) {
      metadata_json->SetStringField(TO_FSTRING(fst), TO_FSTRING(snd));
    }
    json->SetObjectField(TEXT("metadata"), metadata_json);
  }

  if (event.secondary_exposures.has_value()) {
    auto sec_expo_json_arr = secondary_exposures::ToJson(
        event.secondary_exposures.value());
    json->SetArrayField(TEXT("secondaryExposures"), sec_expo_json_arr);
  }

  if (event.string_value.has_value()) {
    json->SetStringField(
        TEXT("value"), FString(event.string_value.value().c_str()));
  } else if (event.double_value.has_value()) {
    json->SetNumberField(TEXT("value"), event.double_value.value());
  }

  return json;
}

inline std::string Serialize(const StatsigEventInternal& event) {
  return unreal_json_utils::JsonObjectToString(ToJson(event));
}

inline StatsigEventInternal Deserialize(const std::string& input) {
  StatsigEventInternal res;
  return res;
}


}
