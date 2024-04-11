#pragma once
#include "json_serialization/unreal_json_utils.hpp"

namespace statsig_compatibility {

class FileHelper {
public:
  static std::string GetCacheDir() {
    const auto dir = GetCacheDirFString();
    return FROM_FSTRING(dir);
  }

  static void WriteStringToFile(
      const std::string& content,
      const std::string& path
      ) {
    FFileHelper::SaveStringToFile(TO_FSTRING(content), *TO_FSTRING(path));
  }

  static std::string CombinePath(const std::string& left,
                                 const std::string& right) {
    auto path = FPaths::Combine(*TO_FSTRING(left), *TO_FSTRING(right));
    return TCHAR_TO_UTF8(*path);
  }

  static std::optional<std::string> ReadStringToFile(const std::string& path) {
    FString result;
    if (FFileHelper::LoadFileToString(result, *TO_FSTRING(path))) {
      return FROM_FSTRING(result);
    }
    return std::nullopt;
  }

  static void EnsureCacheDirectoryExists() {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    auto dir = TO_FSTRING(GetCacheDir());
    if (!manager.DirectoryExists(*dir)) {
      manager.CreateDirectory(*dir);
    }
  }

  static void DeleteFile(const std::string& path) {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    manager.DeleteFile(*TO_FSTRING(path));
  }

  static std::vector<std::string> GetCachePathsSortedYoungestFirst(
      std::string prefix) {
    TArray<FString> paths;
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    FString f_prefix = TO_FSTRING(prefix);

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
      result.push_back(FROM_FSTRING(path));
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
