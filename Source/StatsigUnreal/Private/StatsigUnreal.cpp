#include "StatsigUnreal.h"

#include "Misc/CoreDelegates.h"
#include "Stats/Stats.h"
#include "StatsigClientSettings.h"
#include "FStatsig.h"

#include "statsig.h"

#define LOCTEXT_NAMESPACE "FStatsigUnrealModule"

namespace
{

#if !UE_BUILD_SHIPPING
struct FStatsigUnrealExec : public FSelfRegisteringExec
{
protected:
  virtual bool Exec_Runtime(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override {
    if (FParse::Command(&Cmd, TEXT("STATSIG"))) {
      if (FParse::Command(&Cmd, TEXT("TESTGATE"))) {
        FString GateToTest;
        if (FParse::Token(Cmd, GateToTest, false)) {
          if (FStatsig::CheckGate(GateToTest)) {
              UE_LOG(LogStatsig, Log, TEXT("CheckGate: Gate: %s Result: Pass"), *GateToTest);
            } else {
              UE_LOG(LogStatsig, Log, TEXT("CheckGate: Gate: %s Result: Fail"), *GateToTest);
            }
          }
          return true;
        }
    }
    else if (FParse::Command(&Cmd, TEXT("TESTEXPIRIMENT"))) {
      // todo
    }
    else if (FParse::Command(&Cmd, TEXT("TESTLAYER"))) {
      // todo
    }
    else if (FParse::Command(&Cmd, TEXT("TESTEVENT"))) {
      // todo
    }

    return false;
  }
};
static FStatsigUnrealExec StatsigUnrealExec;
#endif // !UE_BUILD_SHIPPING

}

void FStatsigUnrealModule::StartupModule() {
  EngineLoopInitCompleteDelegate = FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &ThisClass::HandleEngineLoopInitComplete);
}

void FStatsigUnrealModule::ShutdownModule() {
  QUICK_SCOPE_CYCLE_COUNTER(STAT_Statsig_StatsigUnrealModule_ShutdownModule);

  UE_LOG(LogStatsig, Log, TEXT("Shutting down"));

  auto& client = statsig::StatsigClient::Shared();
  client.Shutdown();

  FCoreDelegates::OnFEngineLoopInitComplete.Remove(EngineLoopInitCompleteDelegate);
  FCoreDelegates::GetApplicationWillTerminateDelegate().Remove(AppWillTerminateDelegate);
  FCoreDelegates::ApplicationWillDeactivateDelegate.Remove(AppWillDeactivateDelegate);
  FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Remove(AppSuspendDelegate);
}

void FStatsigUnrealModule::HandleEngineLoopInitComplete()
{
  QUICK_SCOPE_CYCLE_COUNTER(STAT_Statsig_StatsigUnrealModule_HandleEngineLoopInitComplete);

  const UStatsigClientSettings& StatsigClientSettings = *GetDefault<UStatsigClientSettings>();
  UE_LOG(LogStatsig, Log, TEXT("Initializing Client SDK with key: %s"), *StatsigClientSettings.SDKKey);

  statsig::StatsigUser user;

  if (OnOverrideDefaultUserParametersDelegate.IsBound())
  {
    UE_LOG(LogStatsig, Log, TEXT("Overriding default statsig user parameters with custom delegate"));
    OnOverrideDefaultUserParametersDelegate.ExecuteIfBound(user);
  }
  else
  {
    // todo: provide default policy for user id
  }

  const double StartTime = FPlatformTime::Seconds();
  auto& client = statsig::StatsigClient::Shared();

  
  client.InitializeAsync(TCHAR_TO_UTF8(*StatsigClientSettings.SDKKey), [this, StartTime](statsig::StatsigResultCode) {
    UE_LOG(LogStatsig, Log, TEXT("Initialization completed in %f seconds."), FPlatformTime::Seconds() - StartTime);
    AppWillTerminateDelegate = FCoreDelegates::GetApplicationWillTerminateDelegate().AddRaw(this, &ThisClass::HandleAppTerminate);
    AppWillDeactivateDelegate = FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &ThisClass::HandleAppDeactivate);
    AppSuspendDelegate = FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &ThisClass::HandleAppSuspend);
  }, user);
}

void FStatsigUnrealModule::HandleAppTerminate()
{
  QUICK_SCOPE_CYCLE_COUNTER(STAT_Statsig_StatsigUnrealModule_HandleAppTerminate);
  UE_LOG(LogStatsig, Log, TEXT("HandleAppTerminate: Flushing"));
  auto& client = statsig::StatsigClient::Shared();
  client.Flush();
}

void FStatsigUnrealModule::HandleAppDeactivate()
{
  QUICK_SCOPE_CYCLE_COUNTER(STAT_Statsig_StatsigUnrealModule_HandleAppDeactivate);
  UE_LOG(LogStatsig, Log, TEXT("HandleAppDeactivate: Flushing"));
  auto& client = statsig::StatsigClient::Shared();
  client.Flush();
}

void FStatsigUnrealModule::HandleAppSuspend()
{
  QUICK_SCOPE_CYCLE_COUNTER(STAT_Statsig_StatsigUnrealModule_HandleAppSuspend);
  UE_LOG(LogStatsig, Log, TEXT("HandleAppSuspend: Flushing"));
  auto& client = statsig::StatsigClient::Shared();
  client.Flush();
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStatsigUnrealModule, StatsigUnreal)
