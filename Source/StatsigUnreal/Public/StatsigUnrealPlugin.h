#pragma once

#include <string>
#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FStatsigUnrealModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};


