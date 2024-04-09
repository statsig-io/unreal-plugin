#pragma once

#include "json_value.hpp"
#include "unreal_json_utils.hpp"
#include "secondary_exposures_json.hpp"

namespace statsig::data_types {

namespace gate_evaluation {

inline TSharedPtr<FJsonObject> ToJson(const data::GateEvaluation& e) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("name"), TO_FSTRING(e.name));
  json->SetStringField(TEXT("rule_id"), TO_FSTRING(e.rule_id));
  json->SetBoolField(TEXT("value"), e.value);

  if (e.id_type.has_value()) {
    json->SetStringField(TEXT("id_type"), TO_FSTRING(e.id_type.value()));
  }

  auto sec_expo_json_array = secondary_exposures::ToJson(e.secondary_exposures);
  json->SetArrayField(TEXT("secondary_exposures"), sec_expo_json_array);

  return json;
}

inline data::GateEvaluation FromJson(TSharedPtr<FJsonObject> json) {
  data::GateEvaluation e;

  e.name = FROM_FSTRING(json->GetStringField("name"));
  e.rule_id = FROM_FSTRING(json->GetStringField("rule_id"));
  e.value = json->HasField("value") ? json->GetBoolField("value") : false;
  e.secondary_exposures = secondary_exposures::FromJson(
      json, "secondary_exposures");

  return e;
}

}

namespace config_evaluation {

inline TSharedPtr<FJsonObject> ToJson(const data::ConfigEvaluation& e) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("name"), TO_FSTRING(e.name));
  json->SetStringField(TEXT("rule_id"), TO_FSTRING(e.rule_id));
  // json->SetObjectField(TEXT("value"), e.value);

  json->SetStringField(TEXT("id_type"), TO_FSTRING(e.id_type));

  auto sec_expo_json_array = secondary_exposures::ToJson(e.secondary_exposures);
  json->SetArrayField(TEXT("secondary_exposures"), sec_expo_json_array);

  return json;
}

inline data::ConfigEvaluation FromJson(TSharedPtr<FJsonObject> json) {
  data::ConfigEvaluation e;

  e.name = FROM_FSTRING(json->GetStringField("name"));
  e.rule_id = FROM_FSTRING(json->GetStringField("rule_id"));
  e.value = JsonValue(json->GetObjectField("value"));
  e.secondary_exposures = secondary_exposures::FromJson(
      json, "secondary_exposures");
  return e;
}

}


namespace layer_evaluation {

inline TSharedPtr<FJsonObject> ToJson(const data::LayerEvaluation& e) {
  TSharedPtr<FJsonObject> json = MakeShareable(new FJsonObject());

  json->SetStringField(TEXT("name"), TO_FSTRING(e.name));
  json->SetStringField(TEXT("rule_id"), TO_FSTRING(e.rule_id));
  // json->SetObjectField(TEXT("value"), e.value);

  json->SetStringField(TEXT("id_type"), TO_FSTRING(e.id_type));

  auto sec_expo_json_array = secondary_exposures::ToJson(e.secondary_exposures);
  json->SetArrayField(TEXT("secondary_exposures"), sec_expo_json_array);

  auto undel_sec_expo_json_array = secondary_exposures::ToJson(
      e.undelegated_secondary_exposures);
  json->SetArrayField(
      TEXT("undelegated_secondary_exposures"), undel_sec_expo_json_array);

  TArray<TSharedPtr<FJsonValue>> explicit_parameters_json_array;
  for (const auto& param : e.explicit_parameters) {
    sec_expo_json_array.Add(
        MakeShared<FJsonValueString>(TO_FSTRING(param)));
  }
  json->SetArrayField(
      TEXT("explicit_parameters"), explicit_parameters_json_array);

  if (e.allocated_experiment_name.has_value()) {
    json->SetStringField(
        TEXT("allocated_experiment_name"),
        TO_FSTRING(e.allocated_experiment_name.value()));
  }

  return json;
}

inline data::LayerEvaluation FromJson(TSharedPtr<FJsonObject> json) {
  data::LayerEvaluation e;

  e.name = FROM_FSTRING(json->GetStringField("name"));
  e.rule_id = FROM_FSTRING(json->GetStringField("rule_id"));

  for (const auto& param : json->GetArrayField("explicit_parameters")) {
    e.explicit_parameters.push_back(FROM_FSTRING(param->AsString()));
  }

  e.value = JsonValue(json->GetObjectField("value"));
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

inline TSharedPtr<FJsonObject> ToJson(const data::InitializeResponse& res) {
  TSharedPtr<FJsonObject> res_json = MakeShareable(new FJsonObject());

  auto jsonify = [&](const FString& field, const auto& source, auto func) {
    TSharedPtr<FJsonObject> json_obj = MakeShareable(new FJsonObject());
    for (const auto& entry : source) {
      json_obj->SetObjectField(
          TO_FSTRING(entry.first), func(entry.second));
    }
    res_json->SetObjectField(field, json_obj);
  };

  jsonify(TEXT("feature_gates"), res.feature_gates,
          gate_evaluation::ToJson);
  jsonify(TEXT("dynamic_configs"), res.dynamic_configs,
          config_evaluation::ToJson);
  jsonify(TEXT("layer_configs"), res.layer_configs,
          layer_evaluation::ToJson);

  res_json->SetNumberField(TEXT("time"), res.time);

  if (res.generator.has_value()) {
    res_json->SetStringField(
        TEXT("generator"), TO_FSTRING(res.generator.value()));
  }

  return res_json;
}

inline data::InitializeResponse FromJson(const TSharedPtr<FJsonObject>& json) {
  data::InitializeResponse response = {};

  auto parse = [&](const FString& field, auto& target, auto func) {
    const TSharedPtr<FJsonObject>* objectField;
    if (json->TryGetObjectField(field, objectField)) {
      for (const auto& Pair : objectField->Get()->Values) {
        FString Key = Pair.Key;
        const TSharedPtr<FJsonObject> Value = Pair.Value->AsObject();
        const std::string Name = FROM_FSTRING(Key);
        target[Name] = func(Value);
      }
    }
  };

  parse(TEXT("feature_gates"), response.feature_gates,
        gate_evaluation::FromJson);
  parse(TEXT("dynamic_configs"), response.dynamic_configs,
        config_evaluation::FromJson);
  parse(TEXT("layer_configs"), response.layer_configs,
        layer_evaluation::FromJson);

  if (json->HasField(TEXT("time"))) {
    response.time = json->GetNumberField(TEXT("time"));
  }

  if (json->HasField(TEXT("generator"))) {
    response.generator = FROM_FSTRING(json->GetStringField(TEXT("generator")));
  }

  return response;
}

inline std::string Serialize(const data::InitializeResponse& response) {
  return unreal_json_utils::JsonObjectToString(ToJson(response));
}

inline data::InitializeResponse Deserialize(const std::string& input) {
  const auto json = unreal_json_utils::StringToJsonObject(input);
  if (json == nullptr) {
    return {};
  }

  return FromJson(json);
}

}

}
