#include "StatsigTestActorComponent.h"

#include "Engine/Engine.h"
#include "FStatsig.h"

#include "statsig.h"

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
  auto value = values->GetStringField(TEXT("a_string"));
  Log(FString::Format(
          TEXT("an_experiment.a_string: {0} ({1})"),
          {value, experiment.GetEvaluationDetails().reason}),
      FColor::Blue);

  auto not_an_experiment = client.GetExperiment("not_an_experiment");
  auto not_a_value =
      not_an_experiment.GetValues()->GetStringField(TEXT("not_real"));
  Log(FString::Format(
          TEXT("not_an_experiment.not_real: {0} ({1})"),
          {not_a_value,
           not_an_experiment.GetEvaluationDetails().reason}),
      FColor::Blue);

  auto layer = client.GetLayer("a_layer");
  auto param = layer.GetValue("a_string");

  Log(FString::Format(TEXT("a_layer.a_string: {0} ({1})"),
                      {param.has_value() ? param.value()->AsString() : TEXT(""),
                       layer.GetEvaluationDetails().reason}),
      FColor::Blue);

  client.LogEvent({"my_custom_event", "some_value_😎"});

  client.Flush();
}

void UStatsigTestActorComponent::BeginPlay() {
  Super::BeginPlay();

  StatsigUser user;
  user.user_id = UserID;

  StatsigOptions opts;
  opts.output_logger_level = LogLevel::Debug;

  if (bInitializeAsync) {
    FStatsig::InitializeAsync(SDKKey, [](auto) { QueryStatsig(); }, user, opts);
  } else {
    FStatsig::InitializeSync(SDKKey, user, opts);
    QueryStatsig();
  }
}

void UStatsigTestActorComponent::TickComponent(
    float DeltaTime, ELevelTick TickType,
    FActorComponentTickFunction* ThisTickFunction) {
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
