#pragma once

#include "statsig_event_internal.hpp"
#include "unreal_json_utils.hpp"

namespace statsig::data_types::log_event_response {

using LogEventResponse = internal::LogEventResponse;

inline std::optional<LogEventResponse> Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return std::nullopt;
  }

  internal::LogEventResponse response;
  response.success = json->GetBoolField("success");
  return response;
}

}
