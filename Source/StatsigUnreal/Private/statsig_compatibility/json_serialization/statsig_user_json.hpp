#pragma once
#include "unreal_json_utils.hpp"

namespace statsig::data_types::statsig_user {

inline TSharedPtr<FJsonObject> ToJson(const StatsigUser& user) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());
  json->SetStringField(TEXT("userID"), TO_FSTRING(user.user_id));

  json->SetObjectField(
      TEXT("customIDs"),
      unreal_json_utils::UnorderedStringMapJsonObject(
          user.custom_ids));

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
        unreal_json_utils::UnorderedStringMapJsonObject(user.custom.value()));
  }

  if (user.private_attributes.has_value()) {
    json->SetObjectField(
        TEXT("privateAttributes"),
        unreal_json_utils::UnorderedStringMapJsonObject(
            user.private_attributes.value()));
  }

  return json;
}

inline std::string Serialize(const StatsigUser& user) {
  return unreal_json_utils::JsonObjectToString(ToJson(user));
}

inline StatsigUser Deserialize(const std::string& input) {
  TSharedPtr<FJsonObject> json = unreal_json_utils::StringToJsonObject(input);

  auto add_string = [&](const char* field, std::optional<std::string>& target) {
    if (json->HasField(field)) {
      target = FROM_FSTRING(json->GetStringField(field));
    }
  };

  StatsigUser user = {};

  if (json->HasField("userID")) {
    user.user_id = FROM_FSTRING(json->GetStringField("userID"));
  }

  add_string("email", user.email);
  add_string("ip", user.ip);
  add_string("userAgent", user.user_agent);
  add_string("country", user.country);
  add_string("locale", user.locale);
  add_string("appVersion", user.app_version);

  if (json->HasField("custom")) {
    user.custom = unreal_json_utils::JsonObjectToUnorderedMap(json->GetObjectField("custom"));
  }

  if (json->HasField("privateAttributes")) {
    user.private_attributes = unreal_json_utils::JsonObjectToUnorderedMap(json->GetObjectField("privateAttributes"));
  }

  if (json->HasField("customIDs")) {
    user.custom_ids = unreal_json_utils::JsonObjectToUnorderedMap(json->GetObjectField("customIDs"));
  }

  return user;
}

}
