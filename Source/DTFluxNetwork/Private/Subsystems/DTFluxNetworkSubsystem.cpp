// Fill out your copyright notice in the Description page of Project Settings.

#include "Subsystems/DTFluxNetworkSubsystem.h"

#include "DTFluxCoreModule.h"
#include "DTFluxNetworkModule.h"
#include "DTFluxNetworkSettings.h"
#include "JsonObjectConverter.h"
#include "Clients/DTFluxHttpClient.h"
#include "Clients/DTFluxWebSocketClient.h"
#include "Struct/DTFluxServerResponseStruct.h"
#include "Struct/DTFluxRequestStructs.h"
#include "Struct/DTFluxRaceDataServerResponse.h"
#include "Struct/DTFluxTeamListServerResponse.h"
#include "Types/Struct/DTFluxRaceDataStructs.h"


void UFDTFluxNetworkSubsystem::Connect()
{
	WsClient->SetAddress(ConstructWsAddress(WsSettings.Address, WsSettings.Path, WsSettings.Port));
	WsClient->Connect();
}

void UFDTFluxNetworkSubsystem::SendRequest(const EDTFluxRequestType RequestType, int InContestId, int InStageId,
	int InSplitId)
{
	FString Message;
	switch (RequestType)
	{
	case EDTFluxRequestType::ContestRanking:
		FJsonObjectConverter::UStructToJsonObjectString(FDTFluxContestRankingRequest(InContestId), Message);
		break;
	case EDTFluxRequestType::StageRanking:
		FJsonObjectConverter::UStructToJsonObjectString(FDTFluxStageRankingRequest(InContestId, InStageId), Message);
		break;
	case EDTFluxRequestType::SplitRanking:
		FJsonObjectConverter::UStructToJsonObjectString(FDTFluxSplitRankingRequest(InContestId, InStageId, InSplitId), Message);
		break;
	case EDTFluxRequestType::TeamList:
		FJsonObjectConverter::UStructToJsonObjectString(FDTFluxTeamListRequest(), Message);
		break;
	case EDTFluxRequestType::RaceData:
		FJsonObjectConverter::UStructToJsonObjectString(FDTFluxRaceDataRequest(), Message);
		break;
	default:
		return;
	}
	//Dirty trick to fix Case
	Message = Message.Replace(TEXT("Id"),TEXT( "ID"), ESearchCase::CaseSensitive);
	UE_LOG(logDTFluxCore, Warning, TEXT("Sending Request %s"), *Message);
	SendMessage(Message);
}

void UFDTFluxNetworkSubsystem::SendMessage(const FString& Message)
{
	WsClient->Send(Message);
}

void UFDTFluxNetworkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	FDTFluxCoreModule& DTFluxCore = FModuleManager::Get().LoadModuleChecked<FDTFluxCoreModule>("DTFluxCore");
	FString StatusString = UEnum::GetValueAsString(WsStatus);
	UE_LOG(logDTFluxNetwork, Log, TEXT("Status is %s"), *StatusString);
	UDTFluxNetworkSettings* NetworkSettings = GetMutableDefault<UDTFluxNetworkSettings>();
	UDTFluxNetworkSettings::GetWebSocketSettings(NetworkSettings, WsSettings);
	UDTFluxNetworkSettings::GetHTTPSettings(NetworkSettings, HttpSettings);
	WsClient = MakeShareable<FDTFluxWebSocketClient>(new FDTFluxWebSocketClient());
	HttpClient = MakeShareable<FDTFluxHttpClient>(new FDTFluxHttpClient());
	RegisterWebSocketEvents();
	RegisterHttpEvents();
#if WITH_EDITOR
	NetworkSettings->OnDTFluxWebSocketSettingsChanged.AddUFunction(this, FName("WsSettingsChanged"));
	NetworkSettings->OnDTFluxHttpSettingsChanged.AddUFunction(this, FName("HttpSettingsChanged"));
#endif
	if(WsSettings.bShouldConnectAtStartup)
	{
		WsClient->SetAddress(ConstructWsAddress(WsSettings.Address, WsSettings.Path, WsSettings.Port));
		WsClient->Connect();
	}
}

void UFDTFluxNetworkSubsystem::Deinitialize()
{
	Super::Deinitialize();
}

void UFDTFluxNetworkSubsystem::WsSettingsChanged(const FDTFluxWsSettings& NewWsSettings)
{
	// TODO Implement a ClientSelector To retrieve impacted WsClients and populate changes or maybe create a delegate
	bool bNeedsReload = WsSettings != NewWsSettings;

	WsSettings = NewWsSettings;
	// UE_LOG(logDTFluxNetwork, Warning, TEXT("WSocket Settings Changed \n\t Address : %s Path : %s Port : %i\n\tbShouldConnectAtStatup : %s, bShouldAutoReconnectOnClosed %s, bShouldAutoReconnectOnError %s"),
	// 	*NewWsSettings.Address, *NewWsSettings.Path, NewWsSettings.Port,
	// 	NewWsSettings.bShouldConnectAtStartup ? TEXT("True") : TEXT("False"),
	// 	NewWsSettings.bShouldAutoReconnectOnClosed ?  TEXT("True") : TEXT("False"),
	// 	NewWsSettings.bShouldAutoReconnectOnError ?  TEXT("True") : TEXT("False") );
	if( bNeedsReload || WsSettings.bShouldConnectAtStartup)
	{
		UE_LOG(logDTFluxNetwork, Warning, TEXT("WSocket Settings needs Reloding client"))
		ReconnectWs(FName("Ws_Client_0"));
	}
}


void UFDTFluxNetworkSubsystem::HttpSettingsChanged(const FDTFluxHttpSettings& NewHttpSettings)
{
	// TODO Implement a ClientSelector To retrieve impacted HttpClients and populate changes or maybe create a delegate
	HttpSettings = NewHttpSettings;
}

void UFDTFluxNetworkSubsystem::ReconnectWs(const FName WsClientId)
{
	FString NewAddress = ConstructWsAddress(WsSettings.Address, WsSettings.Path, WsSettings.Port);
	WsClient->SetAddress(NewAddress);
	WsClient->Reconnect();
}
void UFDTFluxNetworkSubsystem::ReconnectHttp(const FName WsClientId)
{
	
}

void UFDTFluxNetworkSubsystem::RegisterWebSocketEvents()
{
	OnWsConnectedEventDelegateHandle =
		WsClient->RegisterConnectedEvent().AddUObject(this, &UFDTFluxNetworkSubsystem::OnWebSocketConnected_Subsystem);
	OnWsConnectionErrorEventDelegateHandle =
		WsClient->RegisterConnectionError()
			.AddUObject(this, &UFDTFluxNetworkSubsystem::OnWebSocketConnectionError_Subsystem);
	OnWsClosedEventDelegateHandle = 
		WsClient->RegisterClosedEvent()
		.AddUObject(this, &UFDTFluxNetworkSubsystem::OnWebSocketClosedEvent_Subsystem);
	OnWsMessageEventDelegateHandle =
		WsClient->RegisterMessageEvent()
		.AddUObject(this, &UFDTFluxNetworkSubsystem::OnWebSocketMessageEvent_Subsystem);
	OnWsMessageSentEventDelegateHandle =
	WsClient->RegisterMessageSentEvent()
		.AddUObject(this, &UFDTFluxNetworkSubsystem::OnWebSocketMessageSentEvent_Subsystem);
}

void UFDTFluxNetworkSubsystem::RegisterHttpEvents()
{
}

void UFDTFluxNetworkSubsystem::UnregisterWebSocketEvents()
{
	if(OnWsConnectedEventDelegateHandle.IsValid())
	{
		WsClient->UnregisterConnectedEvent().Remove(OnWsConnectedEventDelegateHandle);
	}
	if(OnWsConnectionErrorEventDelegateHandle.IsValid())
	{
		WsClient->UnregisterConnectionError();
	}
	if(OnWsClosedEventDelegateHandle.IsValid())
	{
		WsClient->UnregisterClosedEvent();
	}
	if(OnWsMessageEventDelegateHandle.IsValid())
	{
		WsClient->UnregisterMessageEvent();
	}
	if(OnWsMessageSentEventDelegateHandle.IsValid())
	{
		WsClient->UnregisterRawMessageEvent();
	}
}

void UFDTFluxNetworkSubsystem::UnregisterHttpEvents()
{
}

void UFDTFluxNetworkSubsystem::OnWebSocketConnected_Subsystem()
{
	OnWebSocketConnected.Broadcast();
	UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws Is Connected with %s"), *WsClient->GetAddress())
}

void UFDTFluxNetworkSubsystem::OnWebSocketConnectionError_Subsystem(const FString& Error)
{
	UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws Error with %s : %s"), *WsClient->GetAddress(), *Error);
	if(WsSettings.bShouldAutoReconnectOnError)
	{
		WsClient->Reconnect();
	}
}

void UFDTFluxNetworkSubsystem::OnWebSocketClosedEvent_Subsystem(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws Error with %s :\n Reason : %s \tStatusCode : %i, bWasClean : %s"),
		*WsClient->GetAddress(), *Reason, StatusCode, bWasClean ? TEXT("True") : TEXT("False"));
}

void UFDTFluxNetworkSubsystem::OnWebSocketMessageEvent_Subsystem(const FString& MessageString)
{
	UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws  %s :\nMessage Received : %s"), *WsClient->GetAddress(), *MessageString);
	//Do Something With the message
	FDTFluxServerResponse Response;
	Response.ReceivedAt = FDateTime::Now();
	Response.RawMessage = MessageString;
	Response.FailureReason = FText::FromString("--");
	if(FJsonObjectConverter::JsonObjectStringToUStruct(MessageString, &Response, 0, 0, false, &(Response.FailureReason)))
	{
		if(Response.Code == -1)
		{
			// return DataReceived.Broadcast(Response);
			if(Response.Type.Contains("race-data"))
			{
				
				FDTFluxRaceDataResponse RaceData;
				FJsonObjectConverter::JsonObjectStringToUStruct<FDTFluxRaceDataResponse>(Response.RawMessage, &RaceData);
				//convert
				FDTFluxRaceData RaceDataDefinition;
				for(auto Contest : RaceData.Datas)
				{
					FDTFluxContest NewContest;
					NewContest.Name = Contest.Name;
					NewContest.ContestId = Contest.Id;
					NewContest.Date = Contest.Date;
					UE_LOG(logDTFluxNetwork, Warning, TEXT("Contest %i [%s] Starting at %s \nStages: \n"), Contest.Id, *Contest.Date.ToString(),*Contest.Name);
					for(auto Stage : Contest.Stages)
					{
						FDTFluxStage NewStage;
						NewStage.StageId = Stage.Id;
						NewStage.Name = Stage.Name;
						FString StartTimeFString = FString::Printf(TEXT("%s %s"),
							*NewContest.Date.ToFormattedString(TEXT("%Y-%m-%d")),
							*Stage.StartTime
							);
						FString EndTimeFString = FString::Printf(TEXT("%s %s"),
							*NewContest.Date.ToFormattedString(TEXT("%Y-%m-%d")),
							*Stage.EndTime
							);
						FString CutOffFString = FString::Printf(TEXT("%s %s"),
							*NewContest.Date.ToFormattedString(TEXT("%Y-%m-%d")),
							*Stage.CutOff
							);
						FDateTime::Parse(StartTimeFString, NewStage.StartTime);
						FDateTime::Parse(EndTimeFString, NewStage.EndTime);
						FDateTime::Parse(CutOffFString, NewStage.CutOff);
						NewContest.Stages.Add(NewStage);
						UE_LOG(logDTFluxNetwork, Warning, TEXT("Stage %i [%s]: \nSTartTime Received [%s] -> Datetime[%s], CutOff [%s], EndTime [%s] \n"), Stage.Id, *Stage.Name,
							*Stage.StartTime, *NewStage.StartTime.ToString(), *NewStage.CutOff.ToString(), *NewStage.EndTime.ToString());
					}
					UE_LOG(logDTFluxNetwork, Warning, TEXT("Contest %i [%s]\nSplits: \n"), Contest.Id, *Contest.Name);
					for(auto Split: Contest.Splits)
					{
						FDTFluxSplit NewSplit;
						NewSplit.SplitId = Split.Id;
						NewSplit.Name = Split.Name;
						NewContest.Splits.Add(NewSplit);
						UE_LOG(logDTFluxNetwork, Warning, TEXT("Split %i [%s]: \n"), Split.Id, *Split.Name);
					}
					RaceDataDefinition.Datas.Add(NewContest);
				}
				UE_LOG(logDTFluxNetwork, Warning, TEXT("Sending %i Contests"), RaceDataDefinition.Datas.Num());
				return OnRaceDataReceived.Broadcast(RaceDataDefinition);
			}
			if(Response.Type.Contains("team-list"))
			{
				UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws Team-List Data"));
				FDTFluxTeamListDefinition TeamListDefinition;
				FJsonObjectConverter::JsonObjectStringToUStruct<FDTFluxTeamListDefinition>(Response.RawMessage, &TeamListDefinition);
				UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws Team-List Data Sent"));
				return OnTeamListReceived.Broadcast(TeamListDefinition);
			}
		}
		UE_LOG(logDTFluxNetwork, Error, TEXT("Ws  %s :\nMessage Cannot be Parsed\n%s"), *WsClient->GetAddress(), *MessageString);

	}
	UE_LOG(logDTFluxNetwork, Error, TEXT("Ws  %s :\nMessage Received : %s Cannot be Parsed"), *WsClient->GetAddress(), *MessageString);
	// return DataReceived.Broadcast(Response);

}

void UFDTFluxNetworkSubsystem::OnWebSocketMessageSentEvent_Subsystem(const FString& MessageSent)
{
	UE_LOG(logDTFluxNetwork, Warning, TEXT("Ws  %s :\nMessage Sent: %s"), *WsClient->GetAddress(), *MessageSent);
}

FString UFDTFluxNetworkSubsystem::ConstructWsAddress(const FString& Address, const FString& Path, const int& Port)
{
	FString NewAddress;
	if( !Address.Contains("ws://") && !Address.Contains("wss://"))
	{
		NewAddress += FString("ws://");
	}
	NewAddress +=Address + FString(":") + FString::FromInt(Port) + Path;
	return NewAddress;
	// UE_LOG(logDTFluxNetwork, Log, TEXT("NewAddress : %s"), *NewAddress);
}
