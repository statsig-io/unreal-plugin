#pragma once
#include "unreal_json_utils.hpp"

namespace statsig::data_types::statsig_user {

inline TSharedPtr<FJsonObject> ToJson(const StatsigUser& user) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());
  json->SetStringField(TEXT("userID"), TO_FSTRING(user.user_id));
  return json;
}

inline std::string Serialize(const StatsigUser& user) {
  return unreal_json_utils::JsonObjectToString(ToJson(user));
}

inline StatsigUser Deserialize(const std::string& input) {
  StatsigUser u = {};
  return u;
}

}
