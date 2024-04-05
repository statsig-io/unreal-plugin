#pragma once

#include <statsig/statsig.h>

class FStatsig
{
public:
	void InitializeSync(const FString& SdkKey);
	bool CheckGate(const FString& GateName);

private:
	statsig::StatsigClient Client;
};
