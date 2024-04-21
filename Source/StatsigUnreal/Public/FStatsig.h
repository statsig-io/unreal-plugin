#pragma once

#include "statsig.h"

class FStatsig {
public:
  static statsig::StatsigResultCode InitializeSync(
      const FString& SdkKey,
      const std::optional<statsig::StatsigUser>& User = std::nullopt,
      const std::optional<statsig::StatsigOptions>& Options = std::nullopt
  );

  static void InitializeAsync(
      const FString& SdkKey,
      const std::function<void(statsig::StatsigResultCode)>& Callback,
      const std::optional<statsig::StatsigUser>& User = std::nullopt,
      const std::optional<statsig::StatsigOptions>& Options = std::nullopt
  );

  static statsig::StatsigResultCode UpdateUserSync(const statsig::StatsigUser &User);

  static void UpdateUserAsync(
    const statsig::StatsigUser &User,
    const std::function<void(statsig::StatsigResultCode)> &Callback
  );

  static bool CheckGate(const FString& GateName);

  static statsig::DynamicConfig GetDynamicConfig(const FString &ConfigName);

  static statsig::Experiment GetExperiment(const FString &ConfigName);

  static statsig::Layer GetLayer(const FString &ConfigName);
};
