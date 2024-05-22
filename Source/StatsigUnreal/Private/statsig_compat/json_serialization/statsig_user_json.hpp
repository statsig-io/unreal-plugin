#pragma once

#include <optional>
#include <string>

#include "unreal_json_utils.hpp"
#include "statsig_compat/primitives/string.hpp"
#include "statsig_result.h"
#include "statsig_user.h"

#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"
#include "Templates/SharedPointer.h"

namespace statsig::data_types::statsig_user {

inline TSharedPtr<FJsonObject> ToJson(const StatsigUser& user) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());
  
  if (GetMapSize(user.custom_ids) > 0) {
    json->SetObjectField(
        TEXT("customIDs"),
        unreal_json_utils::UnorderedStringMapToJsonObject(
            user.custom_ids));
  }

  auto add_string = [&](
      const FString& field,
      const std::optional<String>& source
      ) {
    if (source.has_value()) {
      json->SetStringField(field, source.value());
    }
  };

  add_string("userID", user.user_id);
  add_string("email", user.email);
  add_string("ip", user.ip);
  add_string("userAgent", user.user_agent);
  add_string("country", user.country);
  add_string("locale", user.locale);
  add_string("appVersion", user.app_version);

  if (user.custom.has_value()) {
    json->SetObjectField(
        TEXT("custom"),
        unreal_json_utils::UnorderedStringMapToJsonObject(user.custom.value()));
  }

  if (user.private_attributes.has_value()) {
    json->SetObjectField(
        TEXT("privateAttributes"),
        unreal_json_utils::UnorderedStringMapToJsonObject(
            user.private_attributes.value()));
  }

  return json;
}

inline std::optional<StatsigUser>
FromJson(const TSharedPtr<FJsonObject>& json) {
  auto add_string = [&](const FString &field, std::optional<String>& target) {
    if (json->HasField(field)) {
      target = json->GetStringField(field);
    }
  };

  StatsigUser user = {};

  if (json->HasField(TEXT("userID"))) {
    user.user_id = json->GetStringField(TEXT("userID"));
  }

  add_string(TEXT("email"), user.email);
  add_string(TEXT("ip"), user.ip);
  add_string(TEXT("userAgent"), user.user_agent);
  add_string(TEXT("country"), user.country);
  add_string(TEXT("locale"), user.locale);
  add_string(TEXT("appVersion"), user.app_version);

  if (json->HasField(TEXT("custom"))) {
    user.custom = unreal_json_utils::JsonObjectToUnorderedCompatStringMap(
        json->GetObjectField(TEXT("custom")));
  }

  if (json->HasField(TEXT("privateAttributes"))) {
    user.private_attributes = unreal_json_utils::JsonObjectToUnorderedCompatStringMap(
        json->GetObjectField(TEXT("privateAttributes")));
  }

  if (json->HasField(TEXT("customIDs"))) {
    user.custom_ids = unreal_json_utils::JsonObjectToUnorderedCompatStringMap(
        json->GetObjectField(TEXT("customIDs")));
  }

  return user;
}

inline StatsigResult<std::string> Serialize(const StatsigUser& user) {
  return {Ok, unreal_json_utils::JsonObjectToString(ToJson(user))};
}

inline StatsigResult<StatsigUser> Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return {JsonFailureStatsigUser};
  }

  return {Ok, FromJson(json)};
}

}
