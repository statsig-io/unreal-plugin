#pragma once
#include "json_serialization/unreal_json_utils.hpp"

namespace statsig_compatibility {

class FileHelper {
public:
  static std::string GetCacheDir() {
    const auto dir = GetCacheDirFString();
    return TCHAR_TO_UTF8(*dir);
  }

  static void WriteStringToFile(
      const std::string& content,
      const std::string& path
      ) {
    FFileHelper::SaveStringToFile(FString(content.c_str()),
                                  *FString(path.c_str()));
  }

  static std::string CombinePath(const std::string& left,
                                 const std::string& right) {
    auto path = FPaths::Combine(*FString(left.c_str()),
                                *FString(right.c_str()));
    return TCHAR_TO_UTF8(*path);
  }

  static std::optional<std::string> ReadStringToFile(const std::string& path) {
    FString result;
    if (FFileHelper::LoadFileToString(result, *FString(path.c_str()))) {
      return TCHAR_TO_UTF8(*result);
    }
    return std::nullopt;
  }

  static void EnsureCacheDirectoryExists() {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    auto dir = FString(GetCacheDir().c_str());
    if (!manager.DirectoryExists(*dir)) {
      manager.CreateDirectory(*dir);
    }
  }

  static void DeleteFile(std::string path) {
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    manager.DeleteFile(*FString(path.c_str()));
  }

  static std::vector<std::string> GetCachePathsSortedYoungestFirst(std::string prefix) {
    TArray<FString> paths;
    IPlatformFile& manager = FPlatformFileManager::Get().GetPlatformFile();
    FString f_prefix = FString(prefix.c_str());

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
      result.push_back(TCHAR_TO_UTF8(*path));
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
