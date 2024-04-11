#pragma once


namespace statsig::data_types::error_boundary_request_args {

std::string Serialize(const internal::ErrorBoundaryRequestArgs &args) {
  const TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("exception"), FString(args.exception.c_str()));
  json->SetStringField(TEXT("tag"), FString(args.tag.c_str()));

  TArray<TSharedPtr<FJsonValue>> info_json_arr;
  for (const auto& param : args.info) {
    info_json_arr.Add(MakeShared<FJsonValueString>(FString(param.c_str())));
  }
  json->SetArrayField(TEXT("info"), info_json_arr);

  return unreal_json_utils::JsonObjectToString(json);
}

}