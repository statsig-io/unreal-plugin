#include "StatsigUnreal.h"
#include "FStatsig.h"

#include <iostream>

#define LOCTEXT_NAMESPACE "FStatsigUnrealModule"

void FStatsigUnrealModule::StartupModule() {
  FStatsig Client;
  Client.InitializeSync("client-rfLvYGag3eyU0jYW5zcIJTQip7GXxSrhOFN69IGMjvq");
  // Client.CheckGate("a_gate");

  // This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FStatsigUnrealModule::ShutdownModule() {
  // This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
  // we call this function before unloading the module.
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FStatsigUnrealModule, StatsigUnreal)
