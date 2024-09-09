#pragma once

#include "statsig.h"
#include <functional>
#include <optional>

class FStatsig {
public:
  /// Synchronous initialization from cached or bootstrapped values. (Warn: Will be empty for new users with "NoValues")
  /// @param SdkKey A Statsig Client SDK Key
  /// @param User StatsigUser object to evaluate against
  /// @param Options StatsigOptions used to configure SDK behavior
  /// @return 
  static statsig::StatsigResultCode InitializeSync(
      const FString& SdkKey,
      const std::optional<statsig::StatsigUser>& User = std::nullopt,
      const std::optional<statsig::StatsigOptions>& Options = std::nullopt
  ) {
    return statsig::StatsigClient::Shared().InitializeSync(SdkKey, User, Options);
  }

  /// Asynchronous initialization, fetching latest values from network
  /// @param SdkKey A Statsig Client SDK Key
  /// @param Callback Fired once initialization completes
  /// @param User StatsigUser object to evaluate against
  /// @param Options StatsigOptions used to configure SDK behavior
  static void InitializeAsync(
      const FString& SdkKey,
      const std::function<void(statsig::StatsigResultCode)>& Callback,
      const std::optional<statsig::StatsigUser>& User = std::nullopt,
      const std::optional<statsig::StatsigOptions>& Options = std::nullopt
  ) {
    statsig::StatsigClient::Shared().InitializeAsync(SdkKey, Callback, User, Options);
  }

  /// Updates the internal user object from cached values
  /// @param User StatsigUser object to evaluate against
  static statsig::StatsigResultCode UpdateUserSync(const statsig::StatsigUser& User) {
    return statsig::StatsigClient::Shared().UpdateUserSync(User);
  }

  /// Updates the internal user object from network values
  /// @param User StatsigUser object to evaluate against
  /// @param Callback Fired once the update completes
  static void UpdateUserAsync(
    const statsig::StatsigUser& User,
    const std::function<void(statsig::StatsigResultCode)> &Callback
  ) {
    statsig::StatsigClient::Shared().UpdateUserAsync(User, Callback);
  }

  static void Shutdown() {
    statsig::StatsigClient::Shared().Shutdown();
  }

  static void FlushEvents() {
    statsig::StatsigClient::Shared().Flush();
  }

  static bool CheckGate(const FString& GateName) {
    return statsig::StatsigClient::Shared().CheckGate(GateName);
  }

  static statsig::DynamicConfig GetDynamicConfig(const FString& ConfigName) {
    return statsig::StatsigClient::Shared().GetDynamicConfig(ConfigName);
  }

  static statsig::Experiment GetExperiment(const FString& ConfigName) {
    return statsig::StatsigClient::Shared().GetExperiment(ConfigName);
  }

  static statsig::Layer GetLayer(const FString& ConfigName) {
    return statsig::StatsigClient::Shared().GetLayer(ConfigName);
  }
};
