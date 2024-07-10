#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

namespace statsig { struct StatsigUser; }

class FStatsigUnrealModule : public IModuleInterface {
public:
  using ThisClass = FStatsigUnrealModule;

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;
};
