// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxRankingStructs.h"
#include "DTFluxTeamListStruct.h"
#include "UObject/Object.h"
#include "DTFluxRaceDataStructs.generated.h"


/**
 * Struct representing a Split data
 * Used to exchange data between other objects in the system
 */
USTRUCT(BlueprintType, Category="DTFlux|RaceData")
struct DTFLUXCORE_API FDTFluxSplit
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model", EditAnywhere)
	int SplitId = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model", EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	TArray<FDTFluxSplitRanking> SplitRankings;
	// void Dump() const;
	// // void InsertOrReplace(const FDTFluxStageRankingResponseItem& SplitRankingItemResp);
	// void SortByRank();
	// TArray<FDTFluxSplitRanking> GetSplitRanking(const int From = 0, const int DisplayNumber = 0);

};

/**
 * Struct representing Stage data
 * Used to exchange data between other objects in the system
 */
USTRUCT(BlueprintType, Category="DTFlux|RaceData")
struct DTFLUXCORE_API FDTFluxStage
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	int StageId;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FDateTime StartTime;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FDateTime EndTime;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FDateTime CutOff;
};

/**
 *	Struct representing Contest data
 * Used to exchange data between other objects in the system
 */
USTRUCT(BlueprintType, Category="DTFlux|RaceData")
struct DTFLUXCORE_API FDTFluxContest
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	int ContestId = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString Name;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	TArray<int> ParticipantsBib;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	TArray<FDTFluxStage> Stages;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	TArray<FDTFluxSplit> Splits;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FDateTime Date;
};

USTRUCT()
struct DTFLUXCORE_API FDTFluxRaceData
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxContest> Datas;
};



