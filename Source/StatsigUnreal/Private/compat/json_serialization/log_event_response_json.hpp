#pragma once

#include "statsig_event_internal.hpp"
#include "unreal_json_utils.hpp"

namespace statsig::data_types::log_event_response {

using LogEventResponse = internal::LogEventResponse;

inline StatsigResult<LogEventResponse> Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return {JsonFailureLogEventResponse};
  }

  internal::LogEventResponse response;
  response.success = json->GetBoolField("success");
  return {Ok, response};
}

}
