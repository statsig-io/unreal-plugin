#pragma once

#include <string>

#include "Logging/StructuredLog.h"

namespace statsig_compatibility {

using LogLevel = statsig::LogLevel;

class Log {
public:
  static LogLevel level;

  static void Error(const std::string& value) {
    if (level >= LogLevel::Error) {
      UE_LOGFMT(LogStatsig, Error, "{0}", FString(value.c_str()));
    }
  }

  static void Warn(const std::string& value) {
    if (level >= LogLevel::Warn) {
      UE_LOGFMT(LogStatsig, Warning, "{0}", FString(value.c_str()));
    }
  }

  static void Debug(const std::string& value) {
    if (level >= LogLevel::Debug) {
      UE_LOGFMT(LogStatsig, Log, "{0}", FString(value.c_str()));
    }
  }
};

LogLevel Log::level = LogLevel::Error;

}
