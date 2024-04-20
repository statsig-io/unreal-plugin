#include "Statsig.h"

#include "statsig/statsig.h"

using namespace statsig;

StatsigResultCode FStatsig::InitializeSync(
    const FString& SdkKey,
    const std::optional<StatsigUser>& User,
    const std::optional<StatsigOptions>& Options) {
  return StatsigClient::Shared().InitializeSync(SdkKey, User, Options);
}

void FStatsig::InitializeAsync(
    const FString& SdkKey,
    const std::function<void(StatsigResultCode)>& Callback,
    const std::optional<StatsigUser>& User,
    const std::optional<StatsigOptions>& Options) {
  StatsigClient::Shared().InitializeAsync(SdkKey, Callback, User, Options);
}

bool FStatsig::CheckGate(const FString& GateName) {
  return StatsigClient::Shared().CheckGate(GateName);
}

DynamicConfig FStatsig::GetDynamicConfig(const FString& ConfigName) {
  return StatsigClient::Shared().GetDynamicConfig(ConfigName);
}

Experiment FStatsig::GetExperiment(const FString& ExperimentName) {
  return StatsigClient::Shared().GetExperiment(ExperimentName);

}

Layer FStatsig::GetLayer(const FString& LayerName) {
  return StatsigClient::Shared().GetLayer(LayerName);
}


