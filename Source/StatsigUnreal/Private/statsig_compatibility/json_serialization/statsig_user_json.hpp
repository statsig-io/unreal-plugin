#pragma once

#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "Policies/CondensedJsonPrintPolicy.h"
#include "Serialization/JsonSerializer.h"
#include "Templates/SharedPointer.h"

#include "unreal_json_utils.hpp"

#include <optional>
#include <string>

namespace statsig::data_types::statsig_user {

inline TSharedPtr<FJsonObject> ToJson(const StatsigUser& user) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());
  json->SetStringField(TEXT("userID"), TO_FSTRING(user.user_id));

  if (user.custom_ids.size() > 0) {
    json->SetObjectField(
        TEXT("customIDs"),
        unreal_json_utils::UnorderedStringMapToJsonObject(
            user.custom_ids));
  }

  auto add_string = [&](
      const FString& field,
      const std::optional<std::string>& source
      ) {
    if (source.has_value()) {
      json->SetStringField(field, TO_FSTRING(source.value()));
    }
  };

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
  auto add_string = [&](const char* field, std::optional<std::string>& target) {
    const FString widefield(UTF8_TO_TCHAR(field));
    if (json->HasField(widefield)) {
      target = FROM_FSTRING(json->GetStringField(widefield));
    }
  };

  StatsigUser user = {};

  if (json->HasField(TEXT("userID"))) {
    user.user_id = FROM_FSTRING(json->GetStringField(TEXT("userID")));
  }

  add_string("email", user.email);
  add_string("ip", user.ip);
  add_string("userAgent", user.user_agent);
  add_string("country", user.country);
  add_string("locale", user.locale);
  add_string("appVersion", user.app_version);

  if (json->HasField(TEXT("custom"))) {
    user.custom = unreal_json_utils::JsonObjectToUnorderedStringMap(
        json->GetObjectField(TEXT("custom")));
  }

  if (json->HasField(TEXT("privateAttributes"))) {
    user.private_attributes = unreal_json_utils::JsonObjectToUnorderedStringMap(
        json->GetObjectField(TEXT("privateAttributes")));
  }

  if (json->HasField(TEXT("customIDs"))) {
    user.custom_ids = unreal_json_utils::JsonObjectToUnorderedStringMap(
        json->GetObjectField(TEXT("customIDs")));
  }

  return user;
}

inline std::string Serialize(const StatsigUser& user) {
  return unreal_json_utils::JsonObjectToString(ToJson(user));
}

inline std::optional<StatsigUser> Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return std::nullopt;
  }

  return FromJson(json);
}

}
