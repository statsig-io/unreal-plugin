#pragma once

#include "statsig/statsig.h"

class FStatsig {
public:
  static statsig::StatsigResultCode InitializeSync(
      const FString& SdkKey,
      const std::optional<statsig::StatsigUser>& User = std::nullopt,
      const std::optional<statsig::StatsigOptions>& Options = std::nullopt);

  static void InitializeAsync(
      const FString& SdkKey,
      const std::function<void(statsig::StatsigResultCode)>& Callback,
      const std::optional<statsig::StatsigUser>& User = std::nullopt,
      const std::optional<statsig::StatsigOptions>& Options = std::nullopt);

  static bool CheckGate(const FString& GateName);

  static statsig::DynamicConfig GetDynamicConfig(const FString &ConfigName);

  static statsig::Experiment GetExperiment(const FString &ConfigName);

  static statsig::Layer GetLayer(const FString &ConfigName);
};
