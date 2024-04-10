#pragma once


#include "initialize_request_args.h"

namespace statsig::data_types::initialize_request_args {

inline std::string Serialize(const internal::InitializeRequestArgs &args) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("hash"), FString(args.hash.c_str()));
  json->SetObjectField(TEXT("user"), statsig_user::ToJson(args.user));
  json->SetNumberField(TEXT("sinceTime"), args.since_time);

  const TSharedPtr<FJsonObject> StatsigMetaJson = MakeShareable(new FJsonObject);
  for (const auto& [fst, snd] : args.statsig_metadata)
  {
    StatsigMetaJson->SetStringField(FString(fst.c_str()), FString(snd.c_str()));
  }
  json->SetObjectField(TEXT("statsigMetadata"), StatsigMetaJson);

  return unreal_json_utils::JsonObjectToString(json);
}

}