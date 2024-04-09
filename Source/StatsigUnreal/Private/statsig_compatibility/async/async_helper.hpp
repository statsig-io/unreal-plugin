#pragma once

namespace statsig {

class AsyncHelper {
public:
  static void RunInBackground(std::function<void()> task)
  {
    AsyncTask(ENamedThreads::AnyThread, [task]() {
      task();
    });
  }

};

}
