#pragma once

#include "Engine/DeveloperSettings.h"

#include "StatsigClientSettings.generated.h"

/**
 * Settings for the Statsig Client SDK
 */
UCLASS(config = Engine, meta = (DisplayName = "SDK Settings"))
class UStatsigClientSettings : public UDeveloperSettings {
  GENERATED_BODY()

public:
  UStatsigClientSettings(const FObjectInitializer& ObjectInitializer);

  /** The SDK Key to use when initializing the Statsig Client SDK. */
  UPROPERTY(config, EditAnywhere, Category = "Initialization", meta = (DisplayName = "SDK Key", Tooltip = "Key used to identify the game client with Statsig services."))
  FString SDKKey;
};
