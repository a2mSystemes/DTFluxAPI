// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Struct/DTFluxServerResponseStruct.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/DTFluxNetworkSettingsTypes.h"
#include "Types/Enum/DTfluxCoreEnum.h"
#include "DTFluxNetworkSubsystem.generated.h"




class FDTFluxWebSocketClient;
typedef TSharedPtr<FDTFluxWebSocketClient> FDTFluxWebSocketClientSP;
class FDTFluxHttpClient;
typedef TSharedPtr<FDTFluxHttpClient> FDTFluxHttpClientSP;
/**
 * 
 */
UCLASS(Blueprintable)
class DTFLUXNETWORK_API UFDTFluxNetworkSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()
	
public:

	EDTFluxConnectionStatus WsStatus = EDTFluxConnectionStatus::Unset;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWebSocketConnected);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Network")
	FOnWebSocketConnected OnWebSocketConnected;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRaceDataReceived, const FDTFluxRaceData&, RaceDataDefinition);
	FOnRaceDataReceived OnRaceDataReceived;
	FOnRaceDataReceived& OnReceivedRaceData()
	{
		return OnRaceDataReceived;
	};
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeamListReceived, const FDTFluxTeamListDefinition&, TeamListDefinition);
	FOnTeamListReceived OnTeamListReceived;
	FOnTeamListReceived& OnReceivedTeamList()
	{
		return OnTeamListReceived;
	};

	UFUNCTION(BlueprintCallable, Category="DTFlux|Network")
	void Connect();
	//
	// UFUNCTION(BlueprintCallable, Category="DTFlux|Network")
	// void Reconnect();


	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void SendRequest(const EDTFluxRequestType RequestType, int InContestId = -1, int InStageId = -1, int InSplitId = -1);
	
	
	UFUNCTION(BlueprintCallable,  Category="DTFlux|Network")
	void SendMessage(const FString& Message);
	
protected:
	// ~Subsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~Subsystem Interface


private:
	FDTFluxWsSettings WsSettings;
	FDTFluxHttpSettings HttpSettings;

	UFUNCTION()
	void WsSettingsChanged(const FDTFluxWsSettings& NewWsSettings);
	UFUNCTION()
	void HttpSettingsChanged(const FDTFluxHttpSettings& NewHttpSettings);
	void ReconnectWs(const FName WsClientId);
	void ReconnectHttp(const FName WsClientId);

	void RegisterWebSocketEvents();
	void RegisterHttpEvents();
	void UnregisterWebSocketEvents();
	void UnregisterHttpEvents();

	void OnWebSocketConnected_Subsystem();
	void OnWebSocketConnectionError_Subsystem(const FString& Error);
	void OnWebSocketClosedEvent_Subsystem(int32  StatusCode , const FString& Reason, bool bWasClean);
	void OnWebSocketMessageEvent_Subsystem(const FString& MessageString);
	void OnWebSocketMessageSentEvent_Subsystem(const FString& MessageSent);

	// TODO : Allow multiple instances of network clients.
	// // For Future use of Multi-Connections
	// TArray<FDTFluxWebSocketClientSP> WsClients;
	// // For Future use of Multi-Connections
	// TArray<FDTFluxHttpClientSP> HttpClient;
	// Fo now we jest stick to only one client for each protocol

	FDelegateHandle OnWsConnectedEventDelegateHandle;
	FDelegateHandle OnWsConnectionErrorEventDelegateHandle;
	FDelegateHandle OnWsClosedEventDelegateHandle;
	FDelegateHandle OnWsMessageEventDelegateHandle;
	FDelegateHandle OnWsMessageSentEventDelegateHandle;
	
	FDTFluxWebSocketClientSP WsClient = nullptr;
	FDTFluxHttpClientSP HttpClient = nullptr;


	static FString ConstructWsAddress(const FString& Address, const FString& Path, const int& Port);
};
