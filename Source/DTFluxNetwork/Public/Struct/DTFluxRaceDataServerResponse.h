// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
// #include "DTFluxRaceDataServerResponse.generated.h"
#include "DTFluxRaceDataServerResponse.generated.h"


/**
 * Struct that represent split Json data response from the server
 *  Used to deserialize Split response data 
 */
USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxSplitResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "split-response-data";
	UPROPERTY()
	int Id;
	UPROPERTY()
	FString Name;
};

/**
 * Struct representing Stage data response from the server
 * Used to deserialize Stage response data 
 */
USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxStageResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "stage-response-data";
	UPROPERTY()
	int Id;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString StartTime;
	UPROPERTY()
	FString EndTime;
	UPROPERTY()
	FString CutOff;
};

/**
 * Struct representing Contest data response from the server
 * Used to deserialize Contest Response
 */
USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxContestResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "contest";
	UPROPERTY()
	int Id;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FDateTime Date;
	UPROPERTY()
	TArray<FDTFluxStageResponse> Stages;
	UPROPERTY()
	TArray<FDTFluxSplitResponse> Splits;
};


/**
 *	Struct representing the race data globally
 * Used to exchange data between the system and the API
 */
USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxRaceDataResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	// ReSharper disable once StringLiteralTypo
	FString Type = "race-datas";
	UPROPERTY()
	TArray<FDTFluxContestResponse> Datas;
};