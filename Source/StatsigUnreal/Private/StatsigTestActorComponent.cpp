#include "StatsigTestActorComponent.h"
#include "statsig/statsig.h"

using namespace statsig;

UStatsigTestActorComponent::UStatsigTestActorComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void Log(FString text, FColor color) {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, color, text);
}

void UStatsigTestActorComponent::BeginPlay() {
  Super::BeginPlay();

  StatsigUser user;
  user.user_id = "user-e";
  user.custom_ids["employeeID"] = "employee-e";

  auto& client = StatsigClient::Shared();
  client.InitializeSync(
      "client-rfLvYGag3eyU0jYW5zcIJTQip7GXxSrhOFN69IGMjvq",
      user);

  if (client.CheckGate("a_gate")) {
    Log("a_gate: Pass", FColor::Green);
  } else {
    Log("a_gate: Fail", FColor::Red);
  }

  auto experiment = client.GetExperiment("an_experiment");
  auto values = experiment.GetValues();
  auto value = values->GetStringField("a_string");
  Log(FString::Format(TEXT("an_experiment.a_string: {0}"), {value}),
      FColor::Blue);

  auto not_a_value = values->GetStringField("not_real");
  Log(FString::Format(TEXT("an_experiment.not_real: {0}"), {not_a_value}),
      FColor::Blue);

  auto layer = client.GetLayer("a_layer");
  auto param = layer.GetValue("a_string");

  Log(FString::Format(
          TEXT("a_layer.a_string: {0}"),
          {param.has_value() ? param.value()->AsString() : TEXT("")}),
      FColor::Blue);

  client.Flush();
}


void UStatsigTestActorComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction*
    ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
