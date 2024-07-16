// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Public/Subsystems/EngineSubsystem.h"
// #include "DTFluxWebSocket/DTFluxWebsocketServer.h"
//
// #include "HttpServerRequest.h"
// #include "HttpResultCallback.h"
// #include "HttpRouteHandle.h"
// #include <string>

#include "DTFluxAPILog.h"
#include "DTFluxDataStorage/DTFluxDataStorage.h"
#include "DTFluxModel/DTFluxModel.h"
#include "DTFluxSubsystemAPISettings/DTFluxSubsystemAPISettings.h"
#include "DTFluxWebSocket/DTFluxWebsocketClient.h"
#include "DTFluxSubsystem.generated.h"

class UDTFluxDataStorage;
class UDTFluxProjectSettings;





DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerTriggered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRaceDataReceived);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTimer, FString, TimerName);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWsEvent, FDTFluxWsResponseEvent, WsResponseEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFinisher, FDTFluxSplitRanking, FinisherData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpotter, FDTFluxSplitRanking, SpotterData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSplitSensor, FDTFluxSplitRanking, ParticipantSplitData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnContestBegin, int, ContestId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStageBegin, int, ContestId, int, StageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTimesUp, int, ContestId, int, StageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRestTimeBegin, int, ContestId, int, StageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnArchSelect, int, ContestId, int, StageId);

/**
 * DTFlux API Subsystem
 *
 * This Subsystem set up a Websocket server and Listen to incoming events and poll some http request to Proxy when needed.
 * it handles a datastore where data are being saved and present to blueprint or actors.
 */
UCLASS(BlueprintType, Category="DTFlux|API Subsystem")
class DTFLUXAPI_API UDTFluxSubsystem : public UEngineSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

private:

	static const UDTFluxProjectSettings* GetSettings();
	int WebSocketPort = 0;
	FDTFluxSubsystemAPISettings SubSettings;

	UPROPERTY()
	UDTFluxWebSocketClient* WsClient;
	UPROPERTY()
	UDTFluxDataStorage* DataStorage;
	virtual void Tick(float DeltaTime)  override;

	virtual bool IsTickableInEditor() const override
	{
		return true;
	}
	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UDTFluxSubsystem, STATGROUP_Tickables);
	}


	
protected:
	UFUNCTION()
	void RequestRaceDatas();
	UFUNCTION()
	void RequestTeamList();
	UFUNCTION()
	void RequestContestRanking(const int ContestId);
	UFUNCTION()
	void RequestStageRanking(const int ContestId, const int StageId);
	UFUNCTION()
	void RequestSplitGaps(const int ContestId, const int StageId, const int SplitId);
	UPROPERTY()
	FDateTime TestTimer;
	UFUNCTION()
	void BroadcastTimerEvent();
	UPROPERTY()
	TMap<FDateTime, FOnTimer> Timer;
	bool DataStorageRaceDataInit = false;
	bool DataStorageTeamListInit = false;

public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	void InitDataStorage();
	void LoadConfig(const UDTFluxProjectSettings* Settings);

	UFUNCTION(BlueprintCallable, Category="DTFluxAPI | Subsytem")
	bool ReloadSubsystem();

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnTimerTriggered OnTimerTriggered;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnWsEvent OnWsEvent;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnSplitSensor OnSplitSensor;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnSpotter OnSpotter;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnFinisher OnFinisher;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnContestBegin OnContestBegin;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnStageBegin OnStageBegin;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnTimesUp OnTimesUp;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnRestTimeBegin FOnRestTimeBegin;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnRaceDataReceived OnRaceDataReceived;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnArchSelect OnArchSelect;

	// UPROPERTY(BlueprintReadWrite, Category="DTFlux|Subsystem|Websocket")
	// int ReconnectTimeout = 60; //seconds
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|WebSocket")
	bool Reconnect();
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|WebSocket")
	bool AddTimer(FDateTime Time, FOnTimer NewTimer);
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void SetTimerEvent(const FDateTime& When);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void UpdateRaceData();
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void UpdateTeamList();
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void UpdateTeam();
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void UpdateContestRanking(const int ContestID);
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void UpdateStageRanking(const int ContestID, const int StageID, const int SplitID = -1);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	UDTFluxDataStorage* GetDataStorage()
	{
		return DataStorage;
	};


	UFUNCTION()
	void ProcessTeamListResponse(const FDTFluxTeamListResponse& TeamListResponse);
	UFUNCTION()
	void ProcessRaceDataResponse(const FDTFluxRaceDataResponse& DataResponse);
	UFUNCTION()
	void ProcessContestRankingResponse(const FDTFluxContestRankingResponse& ContestRankingResponse);
	UFUNCTION()
	void ProcessStageRankingResponse(const FDTFluxStageRankingResponse& StageRankingResponse);
	UFUNCTION()
	void ProcessSplitRankingResponse(const FDTFluxStageRankingResponse& StageRankingResponse);
	UFUNCTION()
	void ProcessTeamUpdateResponse(const FDTFluxTeamUpdateResponse& TeamUpdateResponse);
	UFUNCTION()
	void ProcessStatusUpdateResponse(const FDTFluxStatusUpdateResponse& StatusUpdateResponse);
	UFUNCTION()
	void ProcessSplitSensor(const FDTFluxSplitSensorResponse& SplitSensorResponse);

	UFUNCTION()
	void ProcessArchSelect(FDTFluxArchSelectResponseItem ArchSelectResponse);
	
	UFUNCTION()
	void WsConnected();
	UFUNCTION()
	void WsReceivedMessage(const FString& MessageReceived);
	UFUNCTION()
	void WsConnectionClosed(const FString& Reason);
	UFUNCTION()
	void WsConnectionError(const FString& Error);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|subsystem")
	bool IsConnected() const;

};

