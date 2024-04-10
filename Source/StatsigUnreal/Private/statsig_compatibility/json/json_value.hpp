#pragma once

#include <optional>
#include <string>


namespace statsig {

typedef TSharedPtr<FJsonObject> JsonObject;
typedef TSharedPtr<FJsonValue> JsonValue;

inline JsonValue GetJsonValueFromJsonObject(const std::string key, const JsonObject &object) {
  return object->Values[FString(key.c_str())];
}

// class JsonValue: public FJsonObject {
// public:
//   // JsonValue() = default;
//   //
//   // JsonValue(const TSharedPtr<FJsonValue>& input)
//   //   : input {}
//
//   // std::optional<JsonValue> GetValue(const std::string& key) {
//   //   if (raw_value_.IsValid() && raw_value_->Type == EJson::Object) {
//   //     TSharedPtr<FJsonObject> object = value->AsObject();
//   //     exposures.push_back(unreal_json_utils::JsonObjectToUnorderedMap(object));
//   //   }
//   //   
//   //   auto param = FString(key.c_str());
//   //   if (!raw_value_->HasField(param)) {
//   //     return JsonValue(raw_value_->Values[param]);
//   //   }
//   //
//   //   return std::nullopt;
//   // }
//   //
//   // TSharedPtr<FJsonValue> GetRaw() {
//   //   return raw_value_;
//   // }
//
// private:
//   TSharedPtr<FJsonValue> raw_value_;
// };


}
