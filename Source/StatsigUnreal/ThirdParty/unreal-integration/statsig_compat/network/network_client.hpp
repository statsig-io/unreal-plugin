#pragma once

#include <functional>
#include <optional>
#include <string>
#include <unordered_map>

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

namespace statsig::internal {

inline std::unordered_map<std::string, std::string> GetDefaultPlatformHeaders() {
  // no-op - the platform http implementation will add headers for supported encoding types.
  return {};
}

struct HttpResponse {
  const std::string text;
  const int status = -1;
  const std::string sdk_region;
  const std::optional<std::string> error;
};

struct HttpRequest {
  const std::string& api;
  const std::string& path;
  const std::unordered_map<std::string, std::string>& headers;
  const std::string& body;
};

class NetworkClient {
public:
  static void Post(
      const HttpRequest& request,
      const std::function<void(HttpResponse)>& callback
      ) {
    const FString Url = FString((request.api + request.path).c_str());
    const TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().
        CreateRequest();

    HttpRequest->SetURL(Url);
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetContentAsString(request.body.c_str());

    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    for (const auto& [fst, snd] : request.headers) {
      HttpRequest->SetHeader(FString(fst.c_str()), FString(snd.c_str()));
    }

    HttpRequest->SetTimeout(30);

    HttpRequest->OnProcessRequestComplete()
               .BindLambda(
                   [callback](
                   FHttpRequestPtr,
                   const FHttpResponsePtr& Res,
                   const bool bSuccess
                   ) {
                     if (!bSuccess || !Res.IsValid()) {
                       callback({"", -1, "", "FHttpModule::Error::Unknown"});
                       return;
                     }

                     const auto status = Res->GetResponseCode();
                     const auto text = Res->GetContentAsString();

                     std::string region;
                     if (Res->GetHeader("x-statsig-region").Len() > 0) {
                       region = TCHAR_TO_UTF8(* Res->GetHeader("x-statsig-region"));
                     }

                     callback({TCHAR_TO_UTF8(*text), status, region});
                   });

    HttpRequest->ProcessRequest();
  }
};

}
