#pragma once

#include <functional>

#include "internal/shareable.hpp"
#include "statsig_compat/defines/module_definitions.h"

#include "Misc/CoreDelegates.h"
#include "Templates/Function.h"

namespace statsig_compatibility {

class PlatformEventRegistrationHandle final {
public:
  PlatformEventRegistrationHandle() = default;

  PlatformEventRegistrationHandle(const PlatformEventRegistrationHandle&) = delete;
  PlatformEventRegistrationHandle& operator=(const PlatformEventRegistrationHandle&) = delete;

  PlatformEventRegistrationHandle(PlatformEventRegistrationHandle&& Other) {
    unregister_callback = MoveTemp(Other.unregister_callback);
    Other.unregister_callback.Reset();
  }

  PlatformEventRegistrationHandle& operator=(PlatformEventRegistrationHandle&& Other) {
    unregister_callback = MoveTemp(Other.unregister_callback);
    Other.unregister_callback.Reset();
    return *this;
  }

  PlatformEventRegistrationHandle(TUniqueFunction<void()>&& in_unregister_callback)
    : unregister_callback(MoveTemp(in_unregister_callback)) {
  }

  ~PlatformEventRegistrationHandle() {
    Reset();
  }

  void Reset() {
    if (unregister_callback) {
      unregister_callback();
      unregister_callback.Reset();
    }
  }

private:
  TUniqueFunction<void()> unregister_callback;
};

class PlatformEventsHelper {
 public:
  static std::shared_ptr<PlatformEventsHelper> Get(const std::string &sdk_key) {
    auto instance = shareable_.Get(sdk_key);
    if (instance != nullptr) {
      return instance;
    }

    std::shared_ptr<PlatformEventsHelper> new_instance(new PlatformEventsHelper());
    shareable_.Add(sdk_key, new_instance);
    return new_instance;
  }

  PlatformEventRegistrationHandle RegisterOnApplicationWillDeactivateCallback(const std::function<void()> &callback) {
    FDelegateHandle DelegateHandle = FCoreDelegates::ApplicationWillDeactivateDelegate.AddLambda([callback]() {
      if (callback) {
        callback();
      }
    });

    return PlatformEventRegistrationHandle([DelegateHandle]() {
      FCoreDelegates::ApplicationWillDeactivateDelegate.Remove(DelegateHandle);
    });
  }

  PlatformEventRegistrationHandle RegisterOnApplicationWillEnterBackgroundCallback(const std::function<void()> &callback) {
    FDelegateHandle DelegateHandle = FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddLambda([callback]() {
      if (callback) {
        callback();
      }
    });

    return PlatformEventRegistrationHandle([DelegateHandle]() {
      FCoreDelegates::ApplicationWillEnterBackgroundDelegate.Remove(DelegateHandle);
    });
  }

 private:
  STATSIG_EXPORT static statsig::internal::Shareable<PlatformEventsHelper> shareable_;
};

}
