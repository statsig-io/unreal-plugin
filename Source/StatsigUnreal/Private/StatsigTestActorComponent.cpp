#include "StatsigTestActorComponent.h"

#include "json_serialization/unreal_json_utils.hpp"
#include "statsig/statsig.h"

using namespace statsig;

UStatsigTestActorComponent::UStatsigTestActorComponent() {
  PrimaryComponentTick.bCanEverTick = true;
}

void Log(FString text, FColor color) {
  GEngine->AddOnScreenDebugMessage(-1, 5.f, color, text);
}

void QueryStatsig() {
  auto& client = StatsigClient::Shared();

  if (client.CheckGate("a_gate")) {
    Log("a_gate: Pass", FColor::Green);
  } else {
    Log("a_gate: Fail", FColor::Red);
  }

  auto experiment = client.GetExperiment("an_experiment");
  auto values = experiment.GetValues();
  auto value = values->GetStringField("a_string");
  Log(FString::Format(
          TEXT("an_experiment.a_string: {0} ({1})"), {
              value, TO_FSTRING(experiment.GetEvaluationDetails().reason)}),
      FColor::Blue);

  auto not_an_experiment = client.GetExperiment("not_an_experiment");
  auto not_a_value = not_an_experiment.GetValues()->GetStringField("not_real");
  Log(FString::Format(
          TEXT("not_an_experiment.not_real: {0} ({1})"), {
              not_a_value,
              TO_FSTRING(not_an_experiment.GetEvaluationDetails().reason)}),
      FColor::Blue);

  auto layer = client.GetLayer("a_layer");
  auto param = layer.GetValue("a_string");

  Log(FString::Format(
          TEXT("a_layer.a_string: {0} ({1})"),
          {param.has_value() ? param.value()->AsString() : TEXT(""),
           TO_FSTRING(layer.GetEvaluationDetails().reason)}),
      FColor::Blue);

  client.Flush();
}

void UStatsigTestActorComponent::BeginPlay() {
  Super::BeginPlay();

  StatsigUser user;
  user.user_id = FROM_FSTRING(UserID);

  std::string sdk_key = FROM_FSTRING(SDKKey);

  auto& client = StatsigClient::Shared();
  if (bInitializeAsync) {
    client.InitializeAsync(sdk_key, [] { QueryStatsig(); }, user);
  } else {
    client.InitializeSync(sdk_key, user);
    QueryStatsig();
  }
}


void UStatsigTestActorComponent::TickComponent(
    float DeltaTime,
    ELevelTick TickType,
    FActorComponentTickFunction*
    ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
