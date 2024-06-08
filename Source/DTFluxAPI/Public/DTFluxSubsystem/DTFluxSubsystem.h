// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Public/Subsystems/EngineSubsystem.h"
#include "HttpServerRequest.h"
#include "HttpResultCallback.h"
#include "HttpRouteHandle.h"
#include <string>

#include "DTFluxUtils/DTFluxHttpServerStruct.h"
#include "DTFluxAPILog.h"
#include "DTFluxModel/DTFluxModel.h"

#include "DTFluxSubsystem.generated.h"


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




DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FDTFluxOnRequestReceived, FDTFluxHttpServerHeaders, HttpServerHeaders,
                                     FDTFluxHttpServerParams, HttpServerParams, FDTFluxHttpServerBody, HttpRequestBody);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDTFluxOnServerListening);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDTFluxOnServerStopped);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDTFluxOnEventStartReceived, FDTFluxStartStagePayload, Payload);

/**
 * DTFlux API Subsystem
 *
 * This Subsystem Mount HTTP routes to be listened and an HTTP poller to retrieve basic information.
 */
UCLASS()
class DTFLUXAPI_API UDTFluxSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

private:

	bool bIsListening = false;
	const UDTFluxProjectSettings* mSettings;
	
	bool OnRequest(const FHttpServerRequest& Request);

	void HandleRequest(const FString& Route, const FHttpServerRequest& Request, FHttpResultCallback OnComplete);
	// // StartList Payload;
	// FDTFluxStartListPayload StartList;
	// Contest storage
	FDTFluxContestList Contests;
	
	void OnUpdateStartList(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

protected:

	TMap<FString, FHttpRouteHandle> HttpMountedMap;
	TSharedPtr<IHttpRouter>	HttpRouter;
	// Stop The Server
	void StopServer();
	// Create the server response
	TUniquePtr<FHttpServerResponse> CreateHttpServerResponse() const;
	FHttpModule* HttpRequest;

	
public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Subsystem|Events")
	FDTFluxOnRequestReceived OnRequestReceived;

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Subsystem|Events")
	FDTFluxOnServerListening OnServerListening;

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Subsystem|Events")
	FDTFluxOnServerStopped OnServerStopped;

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Subsystem|Events")
	FDTFluxOnEventStartReceived OnEventStartReceived; 
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	TArray<FString> GetMountedRoutes() const;

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem")
	void StartServer();
		
	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|Events")
	TArray<FDTFluxTeam> GetParticipantsByContestId(const int ContestId );

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|Events")
	TArray<FDTFluxTeam> GetParticipantsByContestName(const FString ContestName);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|Events")
	FString GetContestName(const int ContestId);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|Race Result Call")
	void UpdateStartList();

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|Race Result Call")
	void UpdateClassification(const int& ContestId, const int& StageId = -1);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Subsystem|Race Result Call")
	TArray<FDTFluxParticipant> GetClassification(const int& ContestId, const int& StageId = -1);

};

