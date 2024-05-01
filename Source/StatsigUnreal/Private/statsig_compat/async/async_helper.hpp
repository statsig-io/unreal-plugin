#pragma once

#include "Async/Async.h"

#include <functional>

namespace statsig_compatibility {

class AsyncHelper {
public:
  static void RunInBackground(const std::function<void()> &task)
  {
    AsyncTask(ENamedThreads::AnyThread, [task]() {
      task();
    });
  }

  static void Sleep(const long milliseconds) {
    const auto seconds = milliseconds / 1000.0f;
    FPlatformProcess::Sleep(seconds);
  }

};

}
