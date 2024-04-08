#pragma once

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include <functional>

namespace statsig::internal
{
	struct HttpResponse
	{
		const std::string text;
		const int status = -1;
	};

	class NetworkCompat
	{
	public:
		static void Post(
			const std::string& API,
			const std::string& Path,
			const std::unordered_map<std::string, std::string>& Headers,
			const std::string& Body,
			const std::function<void(HttpResponse)>& Callback
		)
		{
			const FString Url = FString((API + Path).c_str());
			const TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();

			HttpRequest->SetURL(Url);
			HttpRequest->SetVerb(TEXT("POST"));
			HttpRequest->SetContentAsString(Body.c_str());

			HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
			for (const auto& [fst, snd] : Headers)
			{
				HttpRequest->SetHeader(FString(fst.c_str()), FString(snd.c_str()));
			}

			HttpRequest->OnProcessRequestComplete().BindLambda(
				[Callback](FHttpRequestPtr, const FHttpResponsePtr& Res, const bool bSuccess) mutable
				{
					if (!bSuccess || !Res.IsValid())
					{
						// Error
						return;
					}

					const auto Status = Res->GetResponseCode();
					if (Status < 200 || Status >= 300)
					{
						// Error
						return;
					}

					const auto Text = Res.Get()->GetContentAsString();
					Callback(HttpResponse{TCHAR_TO_UTF8(*Text), Status});
				});

			HttpRequest->ProcessRequest();
		}
	};
}
