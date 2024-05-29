#pragma once

#include <optional>
#include <string>

#include "statsig_compat/json_serialization/secondary_exposures_json.hpp"
#include "statsig_compat/json_serialization/unreal_json_utils.hpp"
#include "statsig_result.h"

#include "Containers/UnrealString.h"
#include "Dom/JsonObject.h"
#include "Templates/SharedPointer.h"

namespace statsig::data_types {

namespace gate_evaluation {

inline data::GateEvaluation FromJson(TSharedPtr<FJsonObject> json) {
  data::GateEvaluation e;

  e.name = FromCompat(json->GetStringField(TEXT("name")));
  e.rule_id = FromCompat(json->GetStringField(TEXT("rule_id")));
  e.value = json->HasField(TEXT("value")) ? json->GetBoolField(TEXT("value")) : false;
  e.secondary_exposures = secondary_exposures::FromJson(
      json, "secondary_exposures");

  return e;
}

}

namespace config_evaluation {

inline data::ConfigEvaluation FromJson(TSharedPtr<FJsonObject> json) {
  data::ConfigEvaluation e;

  e.name = FromCompat(json->GetStringField(TEXT("name")));
  e.rule_id = FromCompat(json->GetStringField(TEXT("rule_id")));
  
  FString group_name;
  if (json->TryGetStringField(TEXT("group_name"), group_name) && !group_name.IsEmpty()) {
    e.group_name = FromCompat(group_name);
  }

  e.value = json->GetObjectField(TEXT("value"));
  e.secondary_exposures = secondary_exposures::FromJson(
      json, "secondary_exposures");
  return e;
}

}


namespace layer_evaluation {

inline data::LayerEvaluation FromJson(TSharedPtr<FJsonObject> json) {
  data::LayerEvaluation e;

  e.name = FromCompat(json->GetStringField(TEXT("name")));
  e.rule_id = FromCompat(json->GetStringField(TEXT("rule_id")));

  for (const auto& param : json->GetArrayField(TEXT("explicit_parameters"))) {
    e.explicit_parameters.push_back(FromCompat(param->AsString()));
  }

  FString group_name;
  if (json->TryGetStringField(TEXT("group_name"), group_name) && !group_name.IsEmpty()) {
    e.group_name = FromCompat(group_name);
  }

  e.value = json->GetObjectField(TEXT("value"));
  e.secondary_exposures = secondary_exposures::FromJson(
      json, "secondary_exposures");
  e.undelegated_secondary_exposures = secondary_exposures::FromJson(
      json, "undelegated_secondary_exposures");
  e.allocated_experiment_name = unreal_json_utils::TryGetString(
      json, "allocated_experiment_name");

  return e;
}

}

namespace initialize_response {

inline StatsigResult<data::InitializeResponse> FromJson(const TSharedPtr<FJsonObject>& json) {

  auto parse = [&](const FString& field, auto& target, auto func) {
    const TSharedPtr<FJsonObject>* objectField;
    if (json->TryGetObjectField(field, objectField)) {
      for (const auto& Pair : objectField->Get()->Values) {
        FString Key = Pair.Key;
        const TSharedPtr<FJsonObject> Value = Pair.Value->AsObject();
        const std::string Name = FromCompat(Key);
        target[Name] = func(Value);
      }
    }
  };

  if (json->GetBoolField(TEXT("has_updates")) == false) {
    return {Ok, {}};
  }

  if (!unreal_json_utils::HasRequiredFields(json, {"time"})) {
    return {JsonFailureInitializeResponse};
  }

  data::InitializeResponse response{};

  parse(TEXT("feature_gates"), response.feature_gates,
        gate_evaluation::FromJson);
  parse(TEXT("dynamic_configs"), response.dynamic_configs,
        config_evaluation::FromJson);
  parse(TEXT("layer_configs"), response.layer_configs,
        layer_evaluation::FromJson);

  response.time = json->GetNumberField(TEXT("time"));
  response.has_updates = json->GetBoolField(TEXT("has_updates"));

  return {Ok, response};
}

// Do not serialize init res, just use the raw network response string
// inline std::string Serialize(const data::InitializeResponse& response) {
//   return unreal_json_utils::JsonObjectToString(ToJson(response));
// }

inline StatsigResult<data::InitializeResponse> Deserialize(const std::string& input) {
  const auto json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return {JsonFailureInitializeResponse};
  }

  if (!unreal_json_utils::HasRequiredFields(json, {"has_updates"})) {
    return {JsonFailureInitializeResponse};
  }

  return FromJson(json);
}

}

}
