#pragma once

#include <optional>
#include <string>


namespace statsig {

class JsonValue: public FJsonObject {
 public:
  JsonValue() = default;
  JsonValue(const TSharedPtr<FJsonObject>& input) {}

  static TOptional<JsonValue> GetValue(const std::string& key, TSharedPtr<FJsonObject> json) {
    auto param = FString(key.c_str());
    if (!json->HasField(param)) {
      return TOptional<JsonValue>();
    }
    
    return TOptional<JsonValue>();
  }

};


}