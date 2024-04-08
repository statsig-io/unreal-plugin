#pragma once

#include "statsig/statsig_user.h"

namespace statsig::data_types::statsig_user {

TSharedPtr<FJsonObject> ToJson(const StatsigUser &user)
{
  TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
  JsonObject->SetStringField(TEXT("userID"), FString(user.user_id.c_str()));
  return JsonObject;
}

inline std::string Serialize(const StatsigUser &user) {
  const auto JsonObject = ToJson(user);

  FString JsonString;
  const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
  FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

  return TCHAR_TO_UTF8(*JsonString);
}

inline StatsigUser Deserialize(const std::string &input) {
  StatsigUser u;
  return u;
}

}