#pragma once

#include <functional>
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

namespace statsig::internal {

struct HttpResponse {
  const std::string text;
  const int status = -1;
};

class NetworkClient {
 public:
  static void Post(
      const std::string &api,
      const std::string &path,
      const std::unordered_map<std::string, std::string> &headers,
      const std::string &body,
      const std::function<void(HttpResponse)> &callback
  ) {
    const FString Url = FString((api + path).c_str());
    const TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetContentAsString(body.c_str());

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    for (const auto &[fst, snd] : headers) {
      HttpRequest->SetHeader(FString(fst.c_str()), FString(snd.c_str()));
    }

    HttpRequest->SetTimeout(30);

    HttpRequest->OnProcessRequestComplete()
        .BindLambda(
            [callback](
                FHttpRequestPtr,
                const FHttpResponsePtr &Res,
                const bool bSuccess
            ) {
              if (!bSuccess || !Res.IsValid()) {
                callback({});
                return;
              }

              const auto Status = Res->GetResponseCode();
              const auto Text = Res.Get()->GetContentAsString();
              callback({TCHAR_TO_UTF8(*Text), Status});
            });

    HttpRequest->ProcessRequest();
  }
};

}



