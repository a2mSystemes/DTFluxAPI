// Fill out your copyright notice in the Description page of Project Settings.

#include "DTFluxSubsystem/DTFluxSubsystem.h"
#include "DTFluxProjectSettings/DTFluxProjectSettings.h"
#include "DTFluxWebSocket/DTFluxWebsocketServer.h"

#include "DTFluxModel/DTFluxModel.h"
#include "HttpServerModule.h"
#include "HttpRouteHandle.h"
#include "DTFluxAPILog.h"
#include "DTFluxDataStorage/DTFluxDataStorage.h"
#include "IHttpRouter.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpResponse.h"
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetRaceDataEndpoint(const FDTFluxSubsystemAPISettings* Settings)
{
	if(Settings)
	{
		FString RaceDataEndpoint = 
			FString::Printf(TEXT("%s/%p"), *Settings->GetProxyBaseEndpoint(), Settings->ProxyEndpoints.FindKey("race-datas"));
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Race Data -> %s"), *RaceDataEndpoint);
		return RaceDataEndpoint;
	}
	return FString("");
}
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetContestRankingEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId)
{
	if(Settings)
	{
		FString Ranking = *Settings->ProxyEndpoints.FindKey("ranking");
		const TCHAR* ContestIDTmpl = *FString("{:ContestID}");
		const TCHAR* ContestIDValue = *FString(TEXT("%i"),ContestId);
		FString ContestRanking = Ranking.Replace(ContestIDTmpl, ContestIDValue );
		FString ContestRankingEndpoint = Settings->GetProxyBaseEndpoint() + ContestRanking;
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Contest Ranking -> %s"), *ContestRankingEndpoint);
		return ContestRankingEndpoint;
	}
	return FString("");
}
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetStageRankingEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId,
	const int StageId)
{
	if(Settings)
	{
		FString StageRanking = GetContestRankingEndpoint(Settings, ContestId);
		StageRanking = FString::Printf(TEXT("%s/stage/%i/"), *StageRanking, StageId);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Stage Ranking -> %s"), *StageRanking);
		return StageRanking;
	}
	return FString("");
}
// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetStageRankingFilteredEndpoint(const FDTFluxSubsystemAPISettings* Settings,
	const int ContestId, const int StageId, const FString SplitName)
{
	if (Settings){
		FString StageRanking = GetStageRankingEndpoint(Settings, ContestId, StageId);
		StageRanking = FString::Printf(TEXT("%s?splitname=%s"), *StageRanking, *SplitName);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Stage Ranking with Splitname -> %s"), *StageRanking);
		return StageRanking;
	}
	return FString("");
}

// DEPRECATED : Now in WS
FString FDTFluxSubsystemAPISettings::GetTeamsEndpoint(const FDTFluxSubsystemAPISettings* Settings)
{
	if(Settings)
	{
		FString TeamsEndpoint = 
			FString::Printf(TEXT("%s/%p"), *Settings->GetProxyBaseEndpoint(), Settings->ProxyEndpoints.FindKey("teams"));
		UE_LOG(LogDTFluxAPI, Log, TEXT("Proxy Teams -> %s"), *TeamsEndpoint );
		return TeamsEndpoint;
	}
	return FString("");
}

/****
 * DTFlux subsystem 
 ****/


void UDTFluxSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	const UDTFluxProjectSettings* Settings = GetSettings();
	LoadConfig(Settings);
	WsClient = NewObject<UDTFluxWebSocketClient>(this, UDTFluxWebSocketClient::StaticClass());
	WsClient->OnConnectionConnected.AddDynamic(this, &UDTFluxSubsystem::WsConnected );
	WsClient->OnConnectionClosed.AddDynamic(this, &UDTFluxSubsystem::WsConnectionClosed );
	WsClient->OnConnectionError.AddDynamic(this, &UDTFluxSubsystem::WsConnectionError );
	WsClient->OnReceivedMessage.AddDynamic(this, &UDTFluxSubsystem::WsReceivedMessage );
	UE_LOG(LogDTFluxAPI, Log, TEXT("Trying to connect to %s:%i"), *SubSettings.WebsocketAddress, SubSettings.WebsocketPort);
	WsClient->Connect(SubSettings.WebsocketAddress, SubSettings.WebsocketPort);

	DataStorage = NewObject<UDTFluxDataStorage>();
	FDateTime Now = FDateTime::Now();
	FDateTime Send1Min = Now + FTimespan::FromMinutes(1);
	UE_LOG(LogDTFluxAPI, Log, TEXT("TEST timer timeSpan Duration : %s"), *Send1Min.ToString());

	// SetTimerEvent( Send1Min );
	
	// WsServer Event binding
}

void UDTFluxSubsystem::Deinitialize()
{
	if(WsClient)
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("WsClient not null"));
	}
	else
		UE_LOG(LogDTFluxAPI, Log, TEXT("WsClient has been GC'ed"));

	Super::Deinitialize();
}

bool UDTFluxSubsystem::ReloadSubsystem()
{
	return Reconnect();
}

bool UDTFluxSubsystem::Reconnect()
{
	bool Result = WsClient->Close();
	if(!WsClient->IsConnected())
		return WsClient->Connect( SubSettings.WebsocketAddress, SubSettings.WebsocketPort);
	return false;
}

void UDTFluxSubsystem::LoadConfig(const UDTFluxProjectSettings* Settings)
{
	SubSettings.WebsocketPort = Settings->WebsocketServerPort;
	SubSettings.WebsocketAddress = Settings->WebsocketServerAddress;
	SubSettings.ProxyAddress = Settings->ProxyAddress;
	SubSettings.ProxyPort = Settings->ProxyPort;
	TMap<FString,FString> SettingsEndpoints;
	SettingsEndpoints.Add(FString("race-data"), Settings->ProxyRaceDataEndpoint);
	SettingsEndpoints.Add(FString("contest-ranking"), Settings->ProxyRankingEndpoint);
	SettingsEndpoints.Add(FString("stage-ranking"), Settings->ProxyRankingEndpoint);
	SettingsEndpoints.Add(FString("team-list"), Settings->ProxyTeamsEndpoint);
	SubSettings.ProxyEndpoints = SettingsEndpoints;
}

// Get project Settings
const UDTFluxProjectSettings* UDTFluxSubsystem::GetSettings()
{
	if(const UDTFluxProjectSettings* Settings = UDTFluxProjectSettings::GetDTFluxAPIProjectSettings())
		return Settings;
	else
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("Unable to get DTFlux API settings"));
		return nullptr;
	}
}

// tick function
void UDTFluxSubsystem::Tick(float DeltaTime)
{
	if(Timer.Num() > 0)
	{
		TArray<FDateTime> Done;
		for(auto const& El : Timer)
		{
			FDateTime Dt = FDateTime::Now();
			if(Dt >= El.Key)
			{
				El.Value.Execute(TEXT("Tick"));
				OnTimerTriggered.Broadcast();
				UE_LOG(LogDTFluxAPI, Log, TEXT("Execution"));
				UE_LOG(LogDTFluxAPI, Log, TEXT("TICK : exec time: %lld == %lld"), El.Key.GetTicks(), Dt.GetTicks());
				Done.Add(El.Key);
			}

		}
		if(Done.Num() > 0)
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("TICK : Cleaning %i"), Done.Num());
			for(auto const& ToDelete: Done)
			{
				Timer.Remove(ToDelete);
			}
		}
			// UE_LOG(LogDTFluxAPI, Log, TEXT("TICK : Timer Length=%i"), Timer.Num());
	}
}

// TODO: IMPLEMENT THIS METHOD
void UDTFluxSubsystem::WsSplitSensorReceivedInternal()
{
}
// TODO: IMPLEMENT THIS METHOD
void UDTFluxSubsystem::WsTeamUpdateReceivedInternal()
{
}
// TODO: IMPLEMENT THIS METHOD
void UDTFluxSubsystem::WsStatusUpdateReceivedInternal()
{
}

void UDTFluxSubsystem::RequestRaceDatas()
{
	WsClient->SendMessage(TEXT("{\"path\": \"race-datas\"}"));
}

void UDTFluxSubsystem::RequestTeamList()
{
	WsClient->SendMessage(TEXT("{\"path\": \"team-list\"}"));
}

void UDTFluxSubsystem::RequestContestRanking(const int ContestId)
{
	const FString Request = FString::Printf(TEXT("{\"path\": \"contest-ranking\", \"contestID\" : %i}"), ContestId);
	WsClient->SendMessage(Request);
}

void UDTFluxSubsystem::RequestStageRanking(const int ContestId, const int StageId)
{
	const FString Request = FString::Printf(TEXT("{\"path\": \"stage-ranking\", \"contestID\" : %i, \"stageID\" : %i}"), ContestId, StageId);
	WsClient->SendMessage(Request);
}

void UDTFluxSubsystem::RequestSplitGaps(const int ContestId, const int StageId, const int SplitId)
{
	const FString Request =
		FString::Printf(TEXT("{\"path\": \"stage-ranking\", \"contestID\" : %i, \"stageID\" : %i, \"splitID\" : %i}"),
			ContestId, StageId, SplitId);

	WsClient->SendMessage(Request);
}

void UDTFluxSubsystem::UpdateRaceData()
{
	RequestRaceDatas();
}

void UDTFluxSubsystem::UpdateTeamList()
{
	RequestTeamList();
}

void UDTFluxSubsystem::UpdateTeam()
{
}



void UDTFluxSubsystem::UpdateContestRanking(const int ContestID)
{
	RequestContestRanking(ContestID);
}

void UDTFluxSubsystem::UpdateStageRanking(const int ContestID, const int StageID, const int SplitID)
{
	if(SplitID == -1)
	{
		RequestStageRanking(ContestID, StageID);
	}
	else
	{
		RequestSplitGaps(ContestID, StageID, SplitID);
	}
}




EDTFluxResponseType UDTFluxSubsystem::FindResponseType(const FString& MessageReceived)
{
	EDTFluxResponseType ResponseType = UnknownResponse;
	TSharedPtr<FJsonValue> JsonValue;

	// Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(MessageReceived);
	if (FJsonSerializer::Deserialize(Reader, JsonValue))
	{
		// Get the value of the json object by field name
		TSharedPtr<FJsonObject> Json = JsonValue->AsObject();
		//Test
		FString Type = Json->GetStringField(TEXT("type"));
		if(Type == "")
		{
			// UE_LOG(LogDTFluxAPI, Log, TEXT("Type tupe does not exist"));

			return EDTFluxResponseType::UnknownResponse;
		}
		if(Type.Contains("race-datas"))
		{
			// TODO : check if object data are valid
			FDTFluxRaceDataResponse RaceDataResponse;
			if(!FJsonObjectConverter::JsonObjectToUStruct<FDTFluxRaceDataResponse>
				(Json.ToSharedRef(), &RaceDataResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid \"race-data\" object"), *MessageReceived);
				return EDTFluxResponseType::UnknownResponse;
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Message %s is valid race-data object"), *MessageReceived);
			ProcessRaceDataResponse(RaceDataResponse);
			return EDTFluxResponseType::RaceData;
		}
		if(Type.Contains("constest-ranking"))
		{
			FDTFluxContestRankingResponse ContestRankingResponse;
			if(!FJsonObjectConverter::JsonObjectToUStruct<FDTFluxContestRankingResponse>
				(Json.ToSharedRef(), &ContestRankingResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid \"contest-ranking\" object"), *MessageReceived);
			}
			// TODO : check if object data are valid
			UE_LOG(LogDTFluxAPI, Log, TEXT("Message %s is valid \"contest-ranking\" object"), *MessageReceived);
			ProcessContestRankingResponse(ContestRankingResponse);
			return EDTFluxResponseType::ContestRanking;
		}
		if(Type.Contains("stage-ranking"))
		{
			FDTFluxStageRankingResponse StageRankingResponse;
			if(!FJsonObjectConverter::JsonObjectToUStruct<FDTFluxStageRankingResponse>
				(Json.ToSharedRef(), &StageRankingResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid \"stage-ranking\" object"), *MessageReceived);
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Message %s is valid \"stage-ranking\" object"), *MessageReceived);
			if(StageRankingResponse.SplitID == -1)
			{
				ProcessSplitRankingResponse(StageRankingResponse);
				return EDTFluxResponseType::SplitRanking;

			}
			ProcessStageRankingResponse(StageRankingResponse);
			return EDTFluxResponseType::StageRanking;
		}
		if(Type.Contains("team-list"))
		{
			FDTFluxTeamListResponse TeamListResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxTeamListResponse>(Json.ToSharedRef(), &TeamListResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid team-list object"), *MessageReceived)
				return EDTFluxResponseType::UnknownResponse;
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received team-list data"));
			ProcessTeamListResponse(TeamListResponse);
			// TODO : check if object data are valid
			return EDTFluxResponseType::TeamList;
		}
		if(Type.Contains("team-update"))
		{
			// TODO : check if object data are valid
			return EDTFluxResponseType::TeamUpdate;
		}
		if(Type.Contains("split-sensor"))
		{
			// TODO : check if object data are valid
			FDTFluxSplitSensorResponse SplitSensorResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxSplitSensorResponse>(Json.ToSharedRef(), &SplitSensorResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid split-sensor data"), *MessageReceived)
				return EDTFluxResponseType::UnknownResponse;
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received split-sensor data"));
			// send the array to DataStorage;
			return EDTFluxResponseType::SplitSensor;
		}
		if(Type.Contains("status-update"))
		{
			// TODO : check if object data are valid
			return EDTFluxResponseType::StatusUpdate;
		}
	}
	
	return ResponseType;
}

/***
 * Timer handling
 ***/
void UDTFluxSubsystem::BroadcastTimerEvent()
{
	OnTimerTriggered.Broadcast();
	UE_LOG(LogDTFluxAPI, Log, TEXT("TEST timer trigerred at : %s"), *FDateTime::Now().ToString());

}


void UDTFluxSubsystem::SetTimerEvent(const FDateTime& When)
{
	FTimespan TimeSpan = FDateTime::Now() - When;
	UE_LOG(LogDTFluxAPI, Log, TEXT("TEST timer timeSpan Duration : %s"), *TimeSpan.GetDuration().ToString());
}


bool UDTFluxSubsystem::AddTimer(FDateTime Time, FOnTimer NewTimer)
{
	Timer.Add(Time, NewTimer);
	return true;
}


/**
 * END TIMER HANDLING
 ***/

void UDTFluxSubsystem::WsConnected()
{
	OnWsConnected.Broadcast();
	UE_LOG(LogDTFluxAPI, Log, TEXT("Ws Connected"));
}

void UDTFluxSubsystem::WsReceivedMessage( const FString& MessageReceived)
{
	OnWsIncomingData.Broadcast(MessageReceived);
	// UE_LOG(LogDTFluxAPI, Log, TEXT("Ws ReceivedMessage %s"), *MessageReceived);
	// Find Data Object Type
	EDTFluxResponseType Type = FindResponseType(MessageReceived);
	switch(Type)
	{
	case EDTFluxResponseType::TeamList:
		break;
	case EDTFluxResponseType::RaceData:
		break;
	case EDTFluxResponseType::ContestRanking:
		break;
	case EDTFluxResponseType::StageRanking:
		break;
	case EDTFluxResponseType::SplitRanking:
		break;
	case EDTFluxResponseType::TeamUpdate:
		break;
	default:
		break;
	}
	// Let datastorage Know that we received something
	// DataStorage->UpdateDataStorage(MessageReceived);
}

void UDTFluxSubsystem::WsConnectionClosed(const FString& Reason)
{
	OnWsClosed.Broadcast(Reason);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Ws ConnectionClosed with reason %s"), *Reason);
}

void UDTFluxSubsystem::WsConnectionError(const FString& Error)
{
	OnWsError.Broadcast(Error);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Ws Error %s"), *Error);
}

bool UDTFluxSubsystem::IsConnected() const
{
	return WsClient->IsConnected();
}

void UDTFluxSubsystem::ProcessTeamListResponse(const FDTFluxTeamListResponse& TeamListResponse)
{
	for( const auto& TeamListItemResponse : TeamListResponse.Datas)
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Sending Participant %s with Bib %d to DataStorage"), *TeamListItemResponse.LastName, TeamListItemResponse.Bib);

		DataStorage->AddOrUpdateParticipant(TeamListItemResponse);
	}
	for(auto& Contest : DataStorage->Contests)
	{
		Contest.DumpParticipant();
	}
	// UE_LOG(LogDTFluxAPI, Log, TEXT("New Particpant list Size %d"), DataStorage->GetParticipants().Num())

	
}

void UDTFluxSubsystem::ProcessRaceDataResponse(const FDTFluxRaceDataResponse& DataResponse)
{
	for(const auto ContestResponse : DataResponse.Datas)
	{
		DataStorage->AddOrUpdateContest(ContestResponse);
	}

	UE_LOG(LogDTFluxAPI, Log, TEXT("New Contest Size %d"), DataStorage->Contests.Num())

}


void UDTFluxSubsystem::ProcessContestRankingResponse(const FDTFluxContestRankingResponse& ContestRankingResponse)
{
	TArray<FDTFluxContestRanking> NewRankings;
	
	for(const auto& TeamContestRankingResponse : ContestRankingResponse.Datas)
	{
		FDTFluxContestRanking NewRankingEl;
		NewRankingEl.Participant = DataStorage->GetParticipant(ContestRankingResponse.ContestID, TeamContestRankingResponse.Bib);
		NewRankingEl.Rank = TeamContestRankingResponse.Rank;
		NewRankingEl.Gap = TeamContestRankingResponse.Gap;
		NewRankingEl.Time = TeamContestRankingResponse.Time;
		NewRankings.Add(NewRankingEl);
	}
}

void UDTFluxSubsystem::ProcessStageRankingResponse(const FDTFluxStageRankingResponse& StageRankingResponse)
{
}

void UDTFluxSubsystem::ProcessSplitRankingResponse(const FDTFluxStageRankingResponse& SplitRankingResponse)
{
}

void UDTFluxSubsystem::ProcessTeamUpdateResponse(const FDTFluxTeamUpdateResponse& TeamUpdateResponse)
{
}

void UDTFluxSubsystem::ProcessStatusUpdateResponse(const FDTFluxTeamUpdateResponse& TeamUpdateResponse)
{
	
}

void UDTFluxSubsystem::ProcessSplitSensor(const FDTFluxSplitSensorResponse& SplitSensorResponse)
{
}

TSharedPtr<FJsonObject> UDTFluxSubsystem::GetData(EDTFluxResponseType Type, const FString& Message)
{
	TSharedPtr<FJsonValue> JsonValue;
	TSharedPtr<FJsonObject> Object;

	// Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Message);
	if (FJsonSerializer::Deserialize(Reader, JsonValue))
	{
		
	}
	return Object;
}


