#pragma once

#include <nlohmann/json.hpp>

#include "statsig_user_json.hpp"
#include "../../initialize_request_args.h"

namespace statsig::data_types::initialize_request_args {

std::string Serialize(const internal::InitializeRequestArgs &Args) {
  auto j = nlohmann::json{
      {"hash", Args.hash},
      {"user", statsig_user::ToJson(Args.user)},
  };

  return j.dump();
}

}