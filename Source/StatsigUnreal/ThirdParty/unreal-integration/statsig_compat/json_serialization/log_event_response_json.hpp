#pragma once

#include <optional>

#include "internal/statsig_event_internal.hpp"
#include "unreal_json_utils.hpp"

#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"

namespace statsig::data_types::log_event_response {

using LogEventResponse = internal::LogEventResponse;

inline StatsigResult<LogEventResponse> Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return {JsonFailureLogEventResponse};
  }

  internal::LogEventResponse response;
  response.success = json->GetBoolField(TEXT("success"));
  return {Ok, response};
}

}  // namespace statsig::data_types::log_event_response
