#pragma once

#include "sdk/src/initialize_response.hpp"

namespace statsig::data_types
{
	namespace layer_evaluation
	{
		TSharedPtr<FJsonObject> ToJson(const statsig::data::LayerEvaluation& e)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());


			return JsonObject;

			// return nlohmann::json{
			//     {"name", e.name},
			//  {"rule_id", e.rule_id},
			//  {"secondary_exposures", e.secondary_exposures},
			// };
		}

		data::LayerEvaluation FromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			data::LayerEvaluation e;
			//
			// j.at("name").get_to(e.name);
			// j.at("rule_id").get_to(e.rule_id);
			// j.at("secondary_exposures").get_to(e.secondary_exposures);
			// e.value = ValueMap(j["value"]);
			// j.at("explicit_parameters").get_to(e.explicit_parameters);
			// j.at("undelegated_secondary_exposures").get_to(e.undelegated_secondary_exposures);
			//
			// if (j.contains("allocated_experiment_name"))
			// {
			// 	e.allocated_experiment_name = j["allocated_experiment_name"].get<std::string>();
			// }
		
			return e;
		}
	}


	namespace initialize_response
	{
		TSharedPtr<FJsonObject> ToJson(const data::InitializeResponse& res)
		{
			TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());




			return JsonObject;
		}


		data::InitializeResponse FromJson(TSharedPtr<FJsonObject> JsonObject)
		{
			data::InitializeResponse Response = {};

			if (JsonObject->HasField(TEXT("time")))
			{
				Response.time = JsonObject->GetNumberField(TEXT("time"));
			}

			const TSharedPtr<FJsonObject>* LayersObject;
			if (JsonObject->TryGetObjectField(TEXT("layer_configs"), LayersObject)) {
				for (const auto& Pair : LayersObject->Get()->Values) {
					FString Key = Pair.Key;
					const TSharedPtr<FJsonObject> Value = Pair.Value->AsObject();

					const std::string Name = TCHAR_TO_UTF8(*Key);
					Response.layer_configs[Name] = layer_evaluation::FromJson(Value);
				}
			}


			return Response;
		}

		inline std::string Serialize(const statsig::data::InitializeResponse& response)
		{
			const auto JsonObject = initialize_response::ToJson(response);

			FString JsonString;
			const TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
			FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

			return TCHAR_TO_UTF8(*JsonString);
		}

		inline data::InitializeResponse Deserialize(const std::string& input)
		{
			data::InitializeResponse Response = {};

			TSharedPtr<FJsonObject> JsonObject;
			const FString Input = FString(input.c_str());
			const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Input);

			if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
			{
				return Response;
			}

			return FromJson(JsonObject);
		}
	}
}
