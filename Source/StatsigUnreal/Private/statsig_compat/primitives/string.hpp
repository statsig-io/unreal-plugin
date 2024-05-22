#pragma once

#include <string>

#include "Containers/StringConv.h"
#include "Containers/UnrealString.h"

namespace statsig {

typedef FString String;

inline std::string FromCompat(const FString& input) {
  return TCHAR_TO_UTF8(*input);
}

inline String ToCompat(const std::string& input) {
  return FString(UTF8_TO_TCHAR(input.c_str()));
}

}
