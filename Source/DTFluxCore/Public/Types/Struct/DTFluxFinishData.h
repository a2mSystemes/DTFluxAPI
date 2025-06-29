// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxCoreModule.h"
#include "DTFluxRankingStructs.h"
#include "UObject/Object.h"
#include "DTFluxTeamListStruct.h"
#include "DTFluxFinishData.generated.h"


USTRUCT(BlueprintType, Category="DTFlux|Model")
// ReSharper disable once IdentifierTypo
struct DTFLUXCORE_API FDTFluxFinisherData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int ContestId;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int StageId;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxSplitRanking SplitRanking;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxStageRanking StageRanking;
};

USTRUCT(BlueprintType, Category="FDTFlux|Model")
struct DTFLUXCORE_API FDTFluxFinisher
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="FDTFlux|Model")
	EDTFluxFinisherType Type;
	UPROPERTY(BlueprintReadOnly, Category="FDTFlux|Model")
	FDTFluxParticipant Participant;
	UPROPERTY(BlueprintReadOnly, Category="FDTFlux|Model")
	FDTFluxStageRanking CurrentRanking;
};

USTRUCT(BlueprintType, Category="DTFlux|Subsystem|Events")
struct DTFLUXCORE_API FDTFluxStageFinished
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	int ContestId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	int StageId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	TArray<FDTFluxStageRanking> Rankings;
};

USTRUCT(BlueprintType, Category="DTFlux|Subsystem|Events")
struct DTFLUXCORE_API FDTFluxContestFinished
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	int ContestId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	TArray<FDTFluxStageRanking> Rankings;
};

