#include "Statsig.h"

#include <string>
#include <iostream>

void FStatsig::InitializeSync(const FString& SdkKey) {
  statsig::StatsigUser user{"a-user"};
  const std::string Key = TCHAR_TO_UTF8(*SdkKey);
  statsig::StatsigClient::Shared().InitializeAsync(Key, [] {
    std::cout << "Done" << std::endl;
    auto result = statsig::StatsigClient::Shared().CheckGate("a_gate");
    std::cout << "a_gate: " << result << std::endl;

    statsig::StatsigClient::Shared().Flush();
  }, user);
}

bool FStatsig::CheckGate(const FString& GateName) {
  return statsig::StatsigClient::Shared().CheckGate(TCHAR_TO_UTF8(*GateName));  
}
