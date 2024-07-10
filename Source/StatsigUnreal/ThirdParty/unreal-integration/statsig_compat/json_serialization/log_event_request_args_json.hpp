#pragma once

#include <string>

#include "internal/statsig_event_internal.hpp"
#include "statsig_event_json.hpp"
#include "unreal_json_utils.hpp"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Templates/SharedPointer.h"

namespace statsig::data_types::log_event_request_args {

using LogEventRequestArgs = internal::LogEventRequestArgs;

inline StatsigResult<std::string> Serialize(const LogEventRequestArgs& args) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  TArray<TSharedPtr<FJsonValue>> events_json_arr;
  for (const auto& event : args.events) {
    auto event_json = statsig_event::ToJson(event);
    events_json_arr.Add(MakeShared<FJsonValueObject>(event_json));
  }

  json->SetArrayField(TEXT("events"), events_json_arr);

  const TSharedPtr<FJsonObject> statsig_metadata_json = MakeShareable(
      new FJsonObject);
  for (const auto& [fst, snd] : args.statsig_metadata) {
    statsig_metadata_json->SetStringField(ToCompat(fst), ToCompat(snd));
  }
  statsig_metadata_json->SetObjectField(
      TEXT("statsigMetadata"), statsig_metadata_json);

  return {Ok, unreal_json_utils::JsonObjectToString(json)};
}

}
