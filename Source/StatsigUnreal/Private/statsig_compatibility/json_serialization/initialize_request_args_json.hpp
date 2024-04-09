#pragma once


#include "initialize_request_args.h"

namespace statsig::data_types::initialize_request_args {

inline std::string Serialize(const internal::InitializeRequestArgs &args) {
  const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

  JsonObject->SetStringField(TEXT("hash"), FString(args.hash.c_str()));
  JsonObject->SetObjectField(TEXT("user"), statsig_user::ToJson(args.user));

  const TSharedPtr<FJsonObject> StatsigMetaJson = MakeShareable(new FJsonObject);
  for (const auto& [fst, snd] : args.statsig_metadata)
  {
    StatsigMetaJson->SetStringField(FString(fst.c_str()), FString(snd.c_str()));
  }
  JsonObject->SetObjectField(TEXT("statsigMetadata"), StatsigMetaJson);

  FString JsonString;
  const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
  FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

  return TCHAR_TO_UTF8(*JsonString);
}

}