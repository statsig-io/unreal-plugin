#include "Statsig.h"

#include <string>

void FStatsig::InitializeSync(const FString& SdkKey)
{
	const std::string Key = TCHAR_TO_UTF8(*SdkKey);
	Client.InitializeSync(Key);
}

bool FStatsig::CheckGate(const FString& GateName)
{
	auto g = Client.GetFeatureGate(TCHAR_TO_UTF8(*GateName));
	return Client.CheckGate(TCHAR_TO_UTF8(*GateName));
}
