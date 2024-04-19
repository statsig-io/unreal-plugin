#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"
#include "Modules/ModuleManager.h"

namespace statsig { struct StatsigUser; }

class FStatsigUnrealModule : public IModuleInterface {
public:
  using ThisClass = FStatsigUnrealModule;

  /** IModuleInterface implementation */
  virtual void StartupModule() override;
  virtual void ShutdownModule() override;

  DECLARE_DELEGATE_OneParam(FOnOverrideDefaultUserParameters, statsig::StatsigUser& /*User*/);
  FOnOverrideDefaultUserParameters& OnOverrideDefaultUserParameters() { return OnOverrideDefaultUserParametersDelegate; }

private:
  void HandleEngineLoopInitComplete();
  void HandleAppTerminate();
  void HandleAppDeactivate();
  void HandleAppSuspend();

  FDelegateHandle EngineLoopInitCompleteDelegate;
  FDelegateHandle AppWillTerminateDelegate;
  FDelegateHandle AppWillDeactivateDelegate;
  FDelegateHandle AppSuspendDelegate;
  FOnOverrideDefaultUserParameters OnOverrideDefaultUserParametersDelegate;
};
