// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Public/Subsystems/EngineSubsystem.h"
#include "DTFluxWebSocket/DTFluxWebsocketServer.h"

#include "HttpServerRequest.h"
#include "HttpResultCallback.h"
#include "HttpRouteHandle.h"
#include <string>

#include "DTFluxUtils/DTFluxHttpServerStruct.h"
#include "DTFluxAPILog.h"
#include "DTFluxDataStorage/DTFluxDataStorage.h"
#include "DTFluxModel/DTFluxModel.h"
#include "DTFluxWebSocket/DTFluxWebsocketClient.h"
#include "DTFluxSubsystem.generated.h"


class UDTFluxDataStorage;
class UDTFluxProjectSettings;
class IHttpRouter;

class IHttpRequest;
class IHttpResponse;

class FHttpModule;
typedef TSharedPtr<IHttpRequest, ESPMode::ThreadSafe> FHttpRequestPtr;
typedef TSharedPtr<IHttpResponse, ESPMode::ThreadSafe> FHttpResponsePtr;

UENUM(BlueprintType, Category="DTFlux|Server")
enum EDTFluxResponseErrorCode
{
	Unknown_Error UMETA(DisplayName="Unknown Error"),
	InvalidBody_Error UMETA(DisplayName="Invalid Body"),
	InvalidRequest_Error UMETA(DisplayName="Invalid Request"),
	Internal_Error UMETA(DisplayName="Internal Server Error")
};

UENUM(BlueprintType, Category="DTFlux|Subsystem")
enum EDTFluxResponseType: uint8
{
	UnknownResponse = 0 UMETA(DisplayName="UnknownResponse"),
	RaceData = 1 UMETA(DisplayName="RaceData"),
	ContestRanking = 2 UMETA(DisplayName="ContestRanking"),
	StageRanking = 3 UMETA(DisplayName="StageRanking"),
	SplitRanking = 4 UMETA(DisplayName="SplitRanking"),
	TeamList = 5 UMETA(DisplayName="TeamList"),
	TeamUpdate = 6 UMETA(DisplayName="TeamUpdate"),
	SplitSensor = 7 UMETA(DisplayName="SplitSensor"),
	StatusUpdate = 8 UMETA(DisplayName="StatusUpdate"),
};

USTRUCT(BlueprintType, Category="DTFlux|Server")
struct FDTFluxResponseBody
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Error;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Success;

FString Deserialize()
	{
		FString JSONObject;
		JSONObject +=  TEXT("{");
		JSONObject += TEXT("\"error\":\"");
		JSONObject += Error.IsEmpty() ? TEXT("") : Error;
		JSONObject += TEXT("\",\"success\":\"");
		JSONObject += Success.IsEmpty() ? TEXT("") : Success;
		JSONObject += TEXT("\"}");
		UE_LOG(LogDTFluxAPI, Log, TEXT("JSONObject : %s"), *JSONObject);
		return JSONObject;
	}
};


USTRUCT()
struct FDTFluxSubsystemAPISettings
{
	GENERATED_BODY()
public:
	FString WebsocketAddress = "ws://localhost";
	int WebsocketPort = 3000;
	FString ProxyAddress = "http://localhost";
	int ProxyPort = 80;
	//TODO : Maybe we must make a dedicated struct with enum to make endpoints more clean.
	TMap<FString, FString> ProxyEndpoints;

	static  FString GetRaceDataEndpoint(const FDTFluxSubsystemAPISettings* Settings);
	static  FString GetContestRankingEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId);
	static  FString GetStageRankingEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId, const int StageId);
	static  FString GetStageRankingFilteredEndpoint(const FDTFluxSubsystemAPISettings* Settings, const int ContestId, const int StageId, const FString SplitName);
	static  FString GetTeamsEndpoint(const FDTFluxSubsystemAPISettings* Settings);

private:
	FString GetProxyBaseEndpoint() const
	{
		return FString::Printf(TEXT("%s:%i"), *ProxyAddress, ProxyPort);
	};
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWsConnected);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWsIncomingData, FString, JsonData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWsError, FString, Error);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWsClosed, FString, Reason);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTimerTriggered);
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnTimer, FString, TimerName);
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
	void WsSplitSensorReceivedInternal();
	UFUNCTION()
	void WsTeamUpdateReceivedInternal();
	UFUNCTION()
	void WsStatusUpdateReceivedInternal();
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

public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;
	
	UFUNCTION(BlueprintCallable, Category="DTFluxAPI | Subsytem")
	bool ReloadSubsystem();

	void LoadConfig(const UDTFluxProjectSettings* Settings);

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnTimerTriggered OnTimerTriggered;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnWsIncomingData OnWsIncomingData;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnWsConnected OnWsConnected;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnWsError OnWsError;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Events")
	FOnWsClosed OnWsClosed;
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
	void ProcessStatusUpdateResponse(const FDTFluxTeamUpdateResponse& TeamUpdateResponse);
	UFUNCTION()
	void ProcessSplitSensor(const FDTFluxSplitSensorResponse& SplitSensorResponse);

	TSharedPtr<FJsonObject> GetData(EDTFluxResponseType Type, const FString& Message);

	UFUNCTION()
	EDTFluxResponseType FindResponseType(const FString& MessageReceived);
	
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

