#pragma once

#include <functional>
#include <optional>

#include "statsig_compat/output_logger/log.hpp"
#include "statsig_compat/primitives/file_path.hpp"
#include "statsig_compat/primitives/string.hpp"
#include "statsig_options.h"

#include "Async/Async.h"
#include "Containers/UnrealString.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Misc/QueuedThreadPool.h"

namespace statsig_compatibility {

class FileHelper {

public:
  static statsig::FilePath GetCacheDir(const statsig::StatsigOptions &options) {
    if (options.cache_directory.has_value()) {
      return options.cache_directory.value();
    }

    const auto project_dir = FPaths::ProjectPersistentDownloadDir();
    return FPaths::Combine(*project_dir, "Statsig");
  }

  static void WriteStringToFile(
    const std::string &sdk_key,
    const std::string &content,
    const statsig::FilePath &path,
    const std::function<void(bool)> &callback
  ) {
    AsyncPool(*GIOThreadPool, [content, path, callback] {
      FFileHelper::SaveStringToFile(statsig::ToCompat(content), *path);
      Log::Debug("FileHelper: Wrote " + std::to_string(content.length()) + " characters to path " + statsig::FromCompat(path));
      callback(true);
    });
  }

  static statsig::FilePath CombinePath(
    const statsig::FilePath &left,
    const statsig::FilePath &right
  ) {
    const auto path = FPaths::Combine(*left, *right);
    return path;
  }

  static std::optional<std::string> ReadStringToFile(const statsig::FilePath &path) {
    FString result;
    if (FFileHelper::LoadFileToString(result, *path)) {
      Log::Debug("FileHelper: Read " + std::to_string(result.Len()) + " characters from path " + statsig::FromCompat(path));
      return statsig::FromCompat(result);
    }
    return std::nullopt;
  }

  static void EnsureCacheDirectoryExists(
    const statsig::StatsigOptions &options
  ) {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    const auto dir = GetCacheDir(options);
    if (!manager.DirectoryExists(*dir)) {
      manager.CreateDirectory(*dir);
    }
  }

  static void DeleteFile(const statsig::FilePath &path) {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    manager.DeleteFile(*path);
  }

  static std::vector<statsig::FilePath> GetCachePathsSortedYoungestFirst(
    const statsig::StatsigOptions &options,
    std::string prefix
  ) {
    TArray<FString> paths;
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    FString f_prefix = statsig::ToCompat(prefix);

    manager.IterateDirectory(
        *GetCacheDir(options),
        [&paths, &f_prefix](const FString& file_or_dir, bool bIsDirectory) {
          if (bIsDirectory) {
            return true;
          }

          if (!file_or_dir.Contains(f_prefix)) {
            return true;
          }

          paths.Add(file_or_dir);
          return true;
        });

    paths.Sort([&manager](const FString& left, const FString& right) {
      const auto left_time = manager.GetTimeStamp(*left);
      const auto right_time = manager.GetTimeStamp(*right);
      return left_time > right_time;
    });

    std::vector<statsig::FilePath> result;
    for (const FString& path : paths) {
      result.push_back(path);
    }
    return result;
  }
};

}
