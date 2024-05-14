#pragma once

#include "Containers/UnrealString.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

#include "statsig_compat/primitives/string.hpp"

#include <functional>

namespace statsig_compatibility {

class FileHelper {

public:
  static std::string GetCacheDir() {
    const auto dir = GetCacheDirFString();
    return statsig::FromCompat(dir);
  }

  static void WriteStringToFile(
      const std::string &sdk_key,
      const std::string& content,
      const std::string& path,
      const std::function<void(bool)>& callback
      ) {
    AsyncPool(*GIOThreadPool, [content, path, callback] {
      FFileHelper::SaveStringToFile(
          statsig::ToCompat(content),
          *statsig::ToCompat(path)
          );

      callback(true);
    });
  }

  static std::string CombinePath(const std::string& left,
                                 const std::string& right) {
    auto path = FPaths::Combine(*statsig::ToCompat(left),
                                *statsig::ToCompat(right));
    return TCHAR_TO_UTF8(*path);
  }

  static std::optional<std::string> ReadStringToFile(const std::string& path) {
    FString result;
    if (FFileHelper::LoadFileToString(result, *statsig::ToCompat(path))) {
      return statsig::FromCompat(result);
    }
    return std::nullopt;
  }

  static void EnsureCacheDirectoryExists() {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    const auto dir = statsig::ToCompat(GetCacheDir());
    if (!manager.DirectoryExists(*dir)) {
      manager.CreateDirectory(*dir);
    }
  }

  static void DeleteFile(const std::string& path) {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    manager.DeleteFile(*statsig::ToCompat(path));
  }

  static std::vector<std::string> GetCachePathsSortedYoungestFirst(
      std::string prefix) {
    TArray<FString> paths;
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    FString f_prefix = statsig::ToCompat(prefix);

    manager.IterateDirectory(
        *GetCacheDirFString(),
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

    std::vector<std::string> result;
    for (const FString& path : paths) {
      result.push_back(statsig::FromCompat(path));
    }
    return result;
  }

private:
  static FString GetCacheDirFString() {
    const auto project_dir = FPaths::ProjectPersistentDownloadDir();
    return FPaths::Combine(*project_dir, "Statsig");
  }

};

}
