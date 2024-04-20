An Unreal plugin that wraps the Statsig C++ client SDK [https://github.com/statsig-io/cpp-client-sdk](https://github.com/statsig-io/cpp-client-sdk).

## Installation

Clone the statsig-io/unreal-plugin repository into your projects "Plugins" directory.

```
cd path/to/MyUnrealProject/Plugins

git clone --recurse-submodules https://github.com/statsig-io/unreal-plugin
```

## Usage

```
#include "FStatsig.h"

void UNewActorComponent::BeginPlay() {
  Super::BeginPlay();

  statsig::StatsigUser user{TEXT("a-user")};

  FStatsig::InitializeSync(TEXT("YOU_SDK_KEY"), user);

  bool IsGateEnabled = FStatsig::CheckGate(TEXT("a_gate"));
}
```
