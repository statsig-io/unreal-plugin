#include "Statsig.h"

#include <string>

void FStatsig::InitializeSync(const FString& SdkKey)
{
	Client.InitializeSync(TCHAR_TO_UTF8(*SdkKey));
}

bool FStatsig::CheckGate(const FString& GateName)
{
	return Client.CheckGate(TCHAR_TO_UTF8(*GateName));
}
