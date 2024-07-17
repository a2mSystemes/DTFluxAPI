// Fill out your copyright notice in the Description page of Project Settings.

#include "DTFluxSubsystem/DTFluxSubsystem.h"
#include "DTFluxProjectSettings/DTFluxProjectSettings.h"
#include "DTFluxModel/DTFluxModel.h"
#include "DTFluxAPILog.h"
#include "DTFluxDataStorage/DTFluxDataStorage.h"
#include "JsonObjectConverter.h"
#include "DTFluxSubsystemAPISettings/DTFluxSubsystemAPISettings.h"


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
	// DataStorage->InitDatastorage();

	
	
	FDateTime Now = FDateTime::Now();
	FDateTime Send1Min = Now + FTimespan::FromMinutes(1);
	UE_LOG(LogDTFluxAPI, Log, TEXT("TEST timer timeSpan Duration : %s"), *Send1Min.ToString());

}

void UDTFluxSubsystem::Deinitialize()
{
	if(WsClient)
	{
		if (WsClient->Close())
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("WsClient is closed"));
		}
		UE_LOG(LogDTFluxAPI, Error, TEXT("WsClient can not be closed"));
	}
	UE_LOG(LogDTFluxAPI, Log, TEXT("WsClient has been GC'ed"));

	Super::Deinitialize();
}

void UDTFluxSubsystem::InitDataStorage()
{

}

bool UDTFluxSubsystem::ReloadSubsystem()
{
	return Reconnect();
}

bool UDTFluxSubsystem::Reconnect()
{
	bool Result = WsClient->Close();
	DataStorageRaceDataInit = false;
	DataStorageTeamListInit = false;
	if(!WsClient->IsConnected())
		return WsClient->Connect( SubSettings.WebsocketAddress, SubSettings.WebsocketPort);
	return false;
}

void UDTFluxSubsystem::LoadConfig(const UDTFluxProjectSettings* Settings)
{
	SubSettings.WebsocketPort = Settings->WebsocketServerPort;
	SubSettings.WebsocketAddress = Settings->WebsocketServerAddress;
	SubSettings.ProxyAddress = Settings->ProxyAddress;
	// SubSettings.ProxyPort = Settings->ProxyPort;
	// TMap<FString,FString> SettingsEndpoints;
	// SettingsEndpoints.Add(FString("race-data"), Settings->ProxyRaceDataEndpoint);
	// SettingsEndpoints.Add(FString("contest-ranking"), Settings->ProxyRankingEndpoint);
	// SettingsEndpoints.Add(FString("stage-ranking"), Settings->ProxyRankingEndpoint);
	// SettingsEndpoints.Add(FString("team-list"), Settings->ProxyTeamsEndpoint);
	// SubSettings.ProxyEndpoints = SettingsEndpoints;
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
	UE_LOG(LogDTFluxAPI, Log, TEXT("Sending %s to server"), *Request);
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
	FOnTimer NewTimer;
	// NewTimer.BindUFunction()
	// AddTimer(When, )
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
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = EDTFluxResponseType::WsConnected;
	Event.RawData = "Connected";
	if(WsClient->IsConnected())
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Initializing DataStorage"));
		UpdateRaceData();
	}
	OnWsEvent.Broadcast(Event);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Ws Connected"));

}

void UDTFluxSubsystem::WsReceivedMessage( const FString& MessageReceived)
{
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = UnknownResponse;
	Event.RawData = MessageReceived;
	
	TSharedPtr<FJsonValue> JsonValue;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(MessageReceived);
	if (FJsonSerializer::Deserialize(Reader, JsonValue))
	{
		TSharedPtr<FJsonObject> Json = JsonValue->AsObject();
		FString Type = Json->GetStringField(TEXT("type"));
		if(Type.Contains("race-datas"))
		{
			FDTFluxRaceDataResponse RaceDataResponse;
			if(!FJsonObjectConverter::JsonObjectToUStruct<FDTFluxRaceDataResponse>
				(Json.ToSharedRef(), &RaceDataResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid \"race-data\" object"), *MessageReceived);
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Message %s is valid race-data object"), *MessageReceived);
			ProcessRaceDataResponse(RaceDataResponse);
			if(!DataStorageRaceDataInit)
			{
				DataStorageRaceDataInit = true;
				RequestTeamList();
			}
			Event.WsResponseType = RaceData;
		}
		if(Type.Contains("contest-ranking"))
		{
			FDTFluxContestRankingResponse ContestRankingResponse;
			if(!FJsonObjectConverter::JsonObjectToUStruct<FDTFluxContestRankingResponse>
				(Json.ToSharedRef(), &ContestRankingResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid \"contest-ranking\" object"), *MessageReceived);
			}
			ProcessContestRankingResponse(ContestRankingResponse);
			Event.WsResponseType = ContestRanking;

		}
		if(Type.Contains("stage-ranking"))
		{
			FDTFluxStageRankingResponse StageRankingResponse;
			if(!FJsonObjectConverter::JsonObjectToUStruct<FDTFluxStageRankingResponse>
				(Json.ToSharedRef(), &StageRankingResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid \"stage-ranking\" object"), *MessageReceived);
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("\"stage-ranking\" object received"));
			if(StageRankingResponse.SplitID == -1)
			{
				ProcessStageRankingResponse(StageRankingResponse);
				Event.WsResponseType = StageRanking;
			}
			ProcessSplitRankingResponse(StageRankingResponse);
			Event.WsResponseType = SplitRanking;

		}
		if(Type.Contains("team-list"))
		{
			FDTFluxTeamListResponse TeamListResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxTeamListResponse>(Json.ToSharedRef(), &TeamListResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid team-list object"), *MessageReceived)
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received team-list data"));
			ProcessTeamListResponse(TeamListResponse);
			if(!DataStorageTeamListInit)
			{
				DataStorageTeamListInit = true;
				// Initialize contest-rankings
				for(const auto& Contest: DataStorage->Contests)
				{
					RequestContestRanking(Contest.Id);
					// Initialize stage-rankings
					for(const auto Stage : Contest.Stages)
					{
						RequestStageRanking(Contest.Id, Stage.Id);
					}
				}
			}
			Event.WsResponseType = TeamList;
		}
		if(Type.Contains("team-update"))
		{
			FDTFluxTeamUpdateResponse TeamUpdateResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxTeamUpdateResponse>(Json.ToSharedRef(), &TeamUpdateResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid team-update object"), *MessageReceived)
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received team-update data"));
			ProcessTeamUpdateResponse(TeamUpdateResponse);
			Event.WsResponseType = TeamUpdate;
		}
		if(Type.Contains("split-sensor"))
		{
			FDTFluxSplitSensorResponse SplitSensorResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxSplitSensorResponse>(Json.ToSharedRef(), &SplitSensorResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid split-sensor data"), *MessageReceived)
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received split-sensor data"));
			ProcessSplitSensor(SplitSensorResponse);
			Event.WsResponseType = SplitSensor;

		}
		if(Type.Contains("status-update"))
		{
			FDTFluxStatusUpdateResponse StatusUpdateResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxStatusUpdateResponse>(Json.ToSharedRef(), &StatusUpdateResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid status-update data"), *MessageReceived)
			}
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received status-update data %s"), *MessageReceived);
			ProcessStatusUpdateResponse(StatusUpdateResponse);
			Event.WsResponseType = StatusUpdate;

		}
		if(Type.Contains("broadcast-message"))
		{
			FDTFluxArchSelectResponse ArchSelectResponse;
			if( !FJsonObjectConverter::JsonObjectToUStruct
				<FDTFluxArchSelectResponse>(Json.ToSharedRef(), &ArchSelectResponse))
			{
				UE_LOG(LogDTFluxAPI, Error, TEXT("Message %s is not a valid broadcast-message data"), *MessageReceived)
			}
			for(const auto& ArchSelect : ArchSelectResponse.Datas)
			{
				ProcessArchSelect(ArchSelect);
			}
			Event.RawData = "ArchSelect";
			Event.WsResponseType = ArchSelect;
			UE_LOG(LogDTFluxAPI, Log, TEXT("Received broadcast-message data"));
		}
	}
		OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::WsConnectionClosed(const FString& Reason)
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("Ws ConnectionClosed with reason %s trying to reconnect"), *Reason);

	if(!WsClient->IsConnected()){}
	WsClient->Connect( SubSettings.WebsocketAddress, SubSettings.WebsocketPort);
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = WsClosed;
	Event.RawData = Reason;
	OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::WsConnectionError(const FString& Error)
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("Ws Error %s trying to reconnect"), *Error);
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = WsError;
	Event.RawData = Error;
	OnWsEvent.Broadcast(Event);
	bool Result = WsClient->Close();
	DataStorageRaceDataInit = false;
	DataStorageTeamListInit = false;
	if(!WsClient->IsConnected()){}
		WsClient->Connect( SubSettings.WebsocketAddress, SubSettings.WebsocketPort);
}

bool UDTFluxSubsystem::IsConnected() const
{
	return WsClient->IsConnected();
}

void UDTFluxSubsystem::ProcessTeamListResponse(const FDTFluxTeamListResponse& TeamListResponse)
{
	for( const auto& TeamListItemResponse : TeamListResponse.Datas)
	{
		DataStorage->AddOrUpdateParticipant(TeamListItemResponse);
	}
	// for(auto& Contest : DataStorage->Contests)
	// {
	// 	Contest.DumpParticipant();
	// }
	// UE_LOG(LogDTFluxAPI, Log, TEXT("New Particpant list Size %d"), DataStorage->GetParticipants().Num())

	
}

void UDTFluxSubsystem::ProcessRaceDataResponse(const FDTFluxRaceDataResponse& DataResponse)
{
	for(const auto ContestResponse : DataResponse.Datas)
	{
		DataStorage->AddOrUpdateContest(ContestResponse);
	}
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = RaceData;
	Event.RawData = "race-data";
	OnWsEvent.Broadcast(Event);
	OnRaceDataReceived.Broadcast();
	// UE_LOG(LogDTFluxAPI, Log, TEXT("New Contest Size %d"), DataStorage->Contests.Num())

}

void UDTFluxSubsystem::ProcessContestRankingResponse(const FDTFluxContestRankingResponse& ContestRankingResponse)
{
	DataStorage->UpdateContestRanking(ContestRankingResponse);
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = ContestRanking;
	Event.RawData = "contest-ranking";
	OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::ProcessStageRankingResponse(const FDTFluxStageRankingResponse& StageRankingResponse)
{
	DataStorage->UpdateStageRanking(StageRankingResponse);
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = StageRanking;
	Event.RawData = "stage-ranking";
	OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::ProcessSplitRankingResponse(const FDTFluxStageRankingResponse& SplitRankingResponse)
{
	DataStorage->UpdateSplitRanking(SplitRankingResponse);
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = SplitRanking;
	Event.RawData = "split-ranking";
	OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::ProcessTeamUpdateResponse(const FDTFluxTeamUpdateResponse& TeamUpdateResponse)
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("team-update received in c++"));
	for(auto& TeamListRespItem: TeamUpdateResponse.Datas)
	{
		DataStorage->AddOrUpdateParticipant(TeamListRespItem);
	}
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = TeamUpdate;
	Event.RawData = "team-update";
	OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::ProcessStatusUpdateResponse(const FDTFluxStatusUpdateResponse& StatusUpdateResponse)
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("Processing status-update data"));
	DataStorage->UpdateParticipantStatus(StatusUpdateResponse);
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = StatusUpdate;
	Event.RawData = "status-update";
	OnWsEvent.Broadcast(Event);
}

void UDTFluxSubsystem::ProcessSplitSensor(const FDTFluxSplitSensorResponse& SplitSensorResponse)
{
	//

	for(auto& SplitSensorItem : SplitSensorResponse.Datas)
	{
		FDTFluxSplitRanking NewRanking = DataStorage->AddSplitRanking(SplitSensorItem);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Checking SplitStatus ..."))
		EDTFluxSplitType SplitType = DataStorage->GetSplitStatus(SplitSensorItem.ContestID,
			SplitSensorItem.StageID, SplitSensorItem.SplitID);
		FDTFluxFinisherData Data;
		Data.Bib = SplitSensorItem.Bib;
		Data.ContestId = SplitSensorItem.ContestID;
		Data.StageId = SplitSensorItem.StageID;
		Data.SplitRanking = NewRanking;
		switch(SplitType)
		{
		case PreFinnishSplit:

			UE_LOG(LogDTFluxAPI, Warning, TEXT("SplitSensor %d for Stage%02d in Contest%02d is a Prefinish Sensor"),
				SplitSensorItem.SplitID, SplitSensorItem.StageID, SplitSensorItem.ContestID);
			OnSpotter.Broadcast(Data);
			break;
		case FinishSplit:
			UE_LOG(LogDTFluxAPI, Warning, TEXT("SplitSensor %d for Stage%02d in Contest%02d is a Finish Sensor"),
				SplitSensorItem.SplitID, SplitSensorItem.StageID, SplitSensorItem.ContestID);
			DataStorage->GetStageRankingForBib(SplitSensorItem.ContestID, SplitSensorItem.StageID, SplitSensorItem.Bib, Data.StageRanking);
			OnFinisher.Broadcast(Data);
			break;
		case NormalSplit:
			UE_LOG(LogDTFluxAPI, Warning, TEXT("SplitSensor %d for Stage%02d in Contest%02d is a Normal Split"),
			SplitSensorItem.SplitID, SplitSensorItem.StageID, SplitSensorItem.ContestID);
			OnSplitSensor.Broadcast(NewRanking);
			break;
		default:
			UE_LOG(LogDTFluxAPI, Error, TEXT("SplitSensor %d for Stage%02d in Contest%02d %s"),
				SplitSensorItem.SplitID, SplitSensorItem.StageID, SplitSensorItem.ContestID,
				*UEnum::GetValueAsString(SplitType));
			break;
		}
	}
	FDTFluxWsResponseEvent Event;
	Event.WsResponseType = SplitSensor;
	Event.RawData = "split-sensor";
	OnWsEvent.Broadcast(Event);

}

void UDTFluxSubsystem::ProcessArchSelect(FDTFluxArchSelectResponseItem ArchSelectResponse)
{
	OnArchSelect.Broadcast(ArchSelectResponse.ContestId, ArchSelectResponse.StageId);
}

