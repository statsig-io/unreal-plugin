#pragma once

#include "Async/Async.h"

#include <functional>

namespace statsig_compatibility {

class AsyncHelper {
public:
  static std::shared_ptr<AsyncHelper> Get(const std::string& sdk_key) {
    auto instance = shareable_.Get(sdk_key);
    if (instance != nullptr) {
      return instance;
    }

    std::shared_ptr<AsyncHelper> new_instance(new AsyncHelper(sdk_key));
    shareable_.Add(sdk_key, new_instance);
    return new_instance;
  }

  static void Sleep(const long milliseconds) {
    const auto seconds = milliseconds / 1000.0f;
    FPlatformProcess::Sleep(seconds);
  }

  explicit AsyncHelper(const std::string& sdk_key)
    : sdk_key_(sdk_key) {}

  void RunInBackground(const std::function<void()>& task) {
    AsyncTask(ENamedThreads::AnyThread, [task]() {
      task();
    });
  }

  void Start() {
    // noop
  }

  bool Shutdown(const time_t timeout_ms) {
    shareable_.Remove(sdk_key_);
    return true;
  }

private:
  static statsig::internal::Shareable<AsyncHelper> shareable_;

  std::string sdk_key_;
};

statsig::internal::Shareable<AsyncHelper> AsyncHelper::shareable_ =
    statsig::internal::Shareable<AsyncHelper>();

}
