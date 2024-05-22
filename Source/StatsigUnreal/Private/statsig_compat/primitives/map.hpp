#pragma once

#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "statsig_compat/primitives/string.hpp"

#include "Containers/Map.h"

namespace statsig {

typedef TMap<FString, FString> StringMap;

inline std::unordered_map<std::string, std::string> FromCompat(const StringMap& input) {
  std::unordered_map<std::string, std::string> result;
  for (const auto& pair : input) {
    result[FromCompat(pair.Key)] = FromCompat(pair.Value);
  }
  return result;
}

inline StringMap ToCompat(const std::unordered_map<std::string, std::string>& input) {
  StringMap result;
  for (const auto& [fst, snd] : input) {
    result[ToCompat(fst)] = ToCompat(snd);
  }
  return result;
}

inline int GetMapSize(const StringMap &map) {
  return map.Num();
}

inline std::vector<std::pair<String, String>> GetKeyValuePairs(const StringMap &map) {
  std::vector<std::pair<String, String>> ResultVector;
  ResultVector.reserve(map.Num());  // Optimize allocation by reserving space

  for (const auto& pair : map)
  {
    ResultVector.push_back(std::make_pair(pair.Key, pair.Value));
  }

  return ResultVector;

}

}
