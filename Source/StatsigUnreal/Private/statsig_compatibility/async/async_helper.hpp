#pragma once

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
