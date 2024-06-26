#pragma once

#include <functional>

#include "internal/shareable.hpp"
#include "statsig_compat/defines/module_definitions.h"

#include "Async/Async.h"
#include "Containers/Ticker.h"

namespace statsig_compatibility {

class BackgroundTimerHandle final {
public:
  BackgroundTimerHandle() = default;

  BackgroundTimerHandle(const BackgroundTimerHandle&) = delete;
  BackgroundTimerHandle& operator=(const BackgroundTimerHandle&) = delete;

  BackgroundTimerHandle(BackgroundTimerHandle&& Other) {
    timer_delegate_handle = MoveTemp(Other.timer_delegate_handle);
    Other.timer_delegate_handle.Reset();
  }

  BackgroundTimerHandle& operator=(BackgroundTimerHandle&& Other) {
    timer_delegate_handle = MoveTemp(Other.timer_delegate_handle);
    Other.timer_delegate_handle.Reset();
    return *this;
  }

  BackgroundTimerHandle(const TSharedPtr<FTSTicker::FDelegateHandle>& in_timer_delegate_handle)
    : timer_delegate_handle(in_timer_delegate_handle) {
  }

  ~BackgroundTimerHandle() {
    Reset();
  }

  void Reset() {
    if (timer_delegate_handle) {
      FTSTicker::GetCoreTicker().RemoveTicker(*timer_delegate_handle);
      timer_delegate_handle.Reset();
    }
  }

private:
  TSharedPtr<FTSTicker::FDelegateHandle> timer_delegate_handle;
};

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

  explicit AsyncHelper(const std::string& sdk_key)
    : sdk_key_(sdk_key) {}

  void RunInBackground(const std::function<void()>& task) {
    AsyncPool(*GBackgroundPriorityThreadPool, [task]() {
      if (task) {
        task();
      }
    });
  }

  BackgroundTimerHandle StartBackgroundTimer(const std::function<void()>& task, int timer_interval_ms) {
    TSharedRef<FTSTicker::FDelegateHandle> TickDelegateHandle = MakeShared<FTSTicker::FDelegateHandle>();
    ScheduleTimer(task, timer_interval_ms, TickDelegateHandle);
	return BackgroundTimerHandle(TickDelegateHandle);
  }

  void Start() {
    // noop
  }

  bool Shutdown(const time_t timeout_ms) {
    shareable_.Remove(sdk_key_);
    return true;
  }

private:
  static void ScheduleTimer(const std::function<void()>& task, int timer_interval_ms, const TSharedRef<FTSTicker::FDelegateHandle>& tick_delegate_handle)
  {
    const float timer_interval_seconds = timer_interval_ms / 1000.f;

    *tick_delegate_handle = FTSTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateLambda([task, timer_interval_ms, tick_delegate_handle](float) -> bool {
      if (task) {
        task();
      }

      ScheduleTimer(task, timer_interval_ms, tick_delegate_handle);

      // don't auto-reschedule the timer. timers will "catch-up" by firing the callback multiple times if there is a delay.
      return false;
    }), timer_interval_seconds);
  }

  STATSIG_EXPORT static statsig::internal::Shareable<AsyncHelper> shareable_;

  std::string sdk_key_;
};

}
