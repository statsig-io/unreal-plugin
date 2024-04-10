#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "StatsigTestActorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class UStatsigTestActorComponent : public UActorComponent {
  GENERATED_BODY()

public:

  UStatsigTestActorComponent();

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statsig Settings")
  bool bInitializeAsync = true;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Statsig Settings")
  FString SDKKey = "client-rfLvYGag3eyU0jYW5zcIJTQip7GXxSrhOFN69IGMjvq";
  
protected:
  virtual void BeginPlay() override;

public:
  virtual void TickComponent(float DeltaTime, ELevelTick TickType,
                             FActorComponentTickFunction*
                             ThisTickFunction) override;


};
