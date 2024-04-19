#pragma once

#include "Async/Async.h"

#include <functional>

namespace statsig {

class AsyncHelper {
public:
  static void RunInBackground(const std::function<void()> &task)
  {
    AsyncTask(ENamedThreads::AnyThread, [task]() {
      task();
    });
  }

};

}
