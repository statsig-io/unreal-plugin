#include "FStatsig.h"

#include <string>
#include <iostream>
#include <statsig/statsig.h>

void FStatsig::InitializeSync(const FString& SdkKey) {
  statsig::StatsigUser user{"a-user"};
  const std::string Key = TCHAR_TO_UTF8(*SdkKey);
  statsig::StatsigClient::Shared().InitializeAsync(Key, [] {
    std::cout << "Done" << std::endl;
    statsig::StatsigClient& client = statsig::StatsigClient::Shared();
    auto result = client.CheckGate("a_gate");
    std::cout << "a_gate: " << result << std::endl;

    auto config = client.GetExperiment("an_experiment");
    auto values = config.GetValues();
    auto str = values->GetStringField("a_string");
    std::cout << "an_experiment.a_string: " << TCHAR_TO_UTF8(*str) << std::endl;

    statsig::StatsigClient::Shared().Flush();
  }, user);
}

bool FStatsig::CheckGate(const FString& GateName) {
  return statsig::StatsigClient::Shared().CheckGate(TCHAR_TO_UTF8(*GateName));
}
