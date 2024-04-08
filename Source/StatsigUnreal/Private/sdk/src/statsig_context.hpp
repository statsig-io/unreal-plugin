#pragma once

#include <utility>

#include "network_service.hpp"
#include "event_logger.hpp"
#include "evaluation_store.hpp"
#include "statsig/statsig_user.h"
#include "error_boundary.hpp"
#include "statsig_evaluations_data_adapter.hpp"

namespace statsig {

class StatsigContext {
  using NetworkService = statsig::internal::NetworkService;
  using StatsigEvaluationsDataAdapter = statsig::internal::StatsigEvaluationsDataAdapter;
  using EventLogger = statsig::internal::EventLogger;
  using ErrorBoundary = statsig::internal::ErrorBoundary;
  using EvaluationStore = statsig::internal::EvaluationStore;

 public:
  explicit StatsigContext(
      const std::string &sdk_key,
      const std::optional<StatsigUser> &user,
      const std::optional<StatsigOptions> &options
  ) : sdk_key(std::move(sdk_key)),
      user(user.value_or(StatsigUser())),
      options(options.value_or(StatsigOptions())),
      err_boundary(this->sdk_key),
      network(NetworkService(this->sdk_key, this->options)),
      store(EvaluationStore()),
	  logger(EventLogger(this->options, this->network)),
      data_adapter(this->options.data_adapter.value_or(new StatsigEvaluationsDataAdapter())) {
    data_adapter->Attach(this->sdk_key, this->options);
  }

  std::string sdk_key;
  StatsigUser user;
  StatsigOptions options;

  ErrorBoundary err_boundary;
  NetworkService network;
  EvaluationStore store;
  EventLogger logger;
  EvaluationsDataAdapter *data_adapter;
};

}
