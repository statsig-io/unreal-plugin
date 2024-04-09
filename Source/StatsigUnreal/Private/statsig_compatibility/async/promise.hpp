#pragma once

#include <Async/Async.h>

namespace statsig
{
template<typename T>
  class Promise {
  public:
    Promise(TPromise<T> &promise): future_(promise.GetFuture()) {}
  
    void Then(std::function<void(T)> callback) {
      future_.Then(callback);
    }

private:
  TFuture<T> future_;
  };


#pragma once

#include "Async/Async.h"


template<typename ResultType>
class PromiseWrapper
{
public:
  PromiseWrapper() {}

  
  void StartAsyncTask(TFunction<ResultType()> TaskFunction)
  {
    TPromise<ResultType> Promise;
    ResultFuture = Promise.GetFuture();

    Async(EAsyncExecution::ThreadPool, [Promise = MoveTemp(Promise), TaskFunction]() mutable {
        Promise.SetValue(TaskFunction());
    });
  }

  ResultType GetResult() const
  {
    return ResultFuture.Get();
  }

  bool IsReady() const
  {
    return ResultFuture.IsReady();
  }

private:
  TFuture<ResultType> ResultFuture;
};


template <typename T>
class MyAsyncTask
{
public:
  TFuture<T> GetFuture()
  {
    return Promise.GetFuture();
  }

  void Resolve(const T& Value)
  {
    Promise.SetValue(Value);
  }

  // Optionally, you could add functions like Reject() for error handling

private:
  TPromise<T> Promise;
};


}
