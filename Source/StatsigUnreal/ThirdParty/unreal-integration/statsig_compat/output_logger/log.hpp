#pragma once

#include <string>

#include "log_level.h"
#include "statsig_compat/defines/module_definitions.h"

#include "Logging/LogMacros.h"
#include "Logging/StructuredLog.h"

STATSIG_EXPORT DECLARE_LOG_CATEGORY_EXTERN(LogStatsig, Log, All);

namespace statsig_compatibility {

using LogLevel = statsig::LogLevel;

class STATSIG_EXPORT Log {
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

}
