// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Struct/DTFluxRankingStructs.h"
#include "UObject/Object.h"
#include "DTFluxRankingServerResponse.generated.h"


USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxContestRankingResponseItem : public FDTFluxContestRanking
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "team-contest-ranking";
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxContestRankingResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "contest-ranking";
	UPROPERTY()
	int ContestID;
	UPROPERTY()
	TArray<FDTFluxContestRankingResponseItem> Datas;
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxStageRankingResponseItem : public  FDTFluxStageRanking
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "team-stage-ranking";
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxStageRankingResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "stage-ranking";
	UPROPERTY()
	int ContestID;
	UPROPERTY()
	int StageID;
	UPROPERTY()
	int SplitID = -1;
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxStageRankingResponseItem> Datas;
};