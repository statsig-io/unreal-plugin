#pragma once

#include "statsig/evaluations_data_adapter.h"

namespace statsig::data_types::data_adapter_result
{
	inline std::string Serialize(const DataAdapterResult& res)
	{
		const TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

		const auto Source = static_cast<int32>(res.source);

		JsonObject->SetStringField(TEXT("data"), FString(res.data.c_str()));
		JsonObject->SetNumberField(TEXT("source"), Source);
		JsonObject->SetNumberField(TEXT("receivedAt"), res.receivedAt);

		FString JsonString;
		const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

		return TCHAR_TO_UTF8(*JsonString);
	}

	inline DataAdapterResult Deserialize(const std::string& input)
	{
		DataAdapterResult Res = {};
		TSharedPtr<FJsonObject> JsonObject;
		const FString Input = FString(input.c_str());
		const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Input);

		if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
		{
			return Res;
		}

		if (JsonObject->HasField(TEXT("data")))
		{
			const auto Data = JsonObject->GetStringField(TEXT("data"));
			Res.data = TCHAR_TO_UTF8(*Data);
		}

		if (JsonObject->HasField(TEXT("source")))
		{
			int32 SourceInt = JsonObject->GetIntegerField(TEXT("source"));
			Res.source = static_cast<ValueSource>(SourceInt);
		}

		if (JsonObject->HasField(TEXT("receivedAt")))
		{
			Res.receivedAt = JsonObject->GetNumberField(TEXT("receivedAt"));
		}

		return Res;
	}
}
