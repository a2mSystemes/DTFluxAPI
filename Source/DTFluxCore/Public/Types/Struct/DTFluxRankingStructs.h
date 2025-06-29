// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxRankingStructs.generated.h"


/**
 * @struct FDTFluxContestRanking
 * Representing a contest ranking for a participant 
 */
USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXCORE_API FDTFluxContestRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Time;
	UPROPERTY();
	FString SpeedSwimAverage;
	UPROPERTY();
	FString SpeedRunningAverage;
	UPROPERTY();
	FString SpeedTotalAverage;
	void Dump () const;

};


/**
 * @struct FDTFluxStageRanking
 * Representing a stage ranking for a participant
 */
USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXCORE_API FDTFluxStageRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Time;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeSwim;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeTransition;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeRun;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeStart;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDateTime StartTime;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	float SpeedRunning;	
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	float SpeedTotal;	
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	float SpeedSwim;
	void Dump() const;

};

/**
 * @struct FDTFluxSplitRanking
 * Representing a ranking of a participant in a split
 */
USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXCORE_API FDTFluxSplitRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int ContestId = 0;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int StageId = 0;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int SplitId = 0;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Time;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank = 0;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	bool Display = false;
	void Dump() const;

};