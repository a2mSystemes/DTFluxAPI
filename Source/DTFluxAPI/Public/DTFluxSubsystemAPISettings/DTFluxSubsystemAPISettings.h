// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxSubsystemAPISettings.generated.h"

/**
 * 
 */
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
