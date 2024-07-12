// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxModelResponse.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FSplitResponse
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


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FStageResponse
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

};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxContestResponse
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
	FString Date;
	UPROPERTY()
	TArray<FStageResponse> Stages;
	UPROPERTY()
	TArray<FSplitResponse> Splits;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxContestRankingResponseItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "team-contest-ranking";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	int Rank;
	UPROPERTY()
	FString Time;
	UPROPERTY();
	FString Gap;
	UPROPERTY();
	FString SpeedSwimAverage;
	UPROPERTY();
	FString SpeedRunningAverage;
	UPROPERTY();
	FString SpeedTotalAverage;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxContestRankingResponse
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

USTRUCT()
struct DTFLUXAPI_API FDTFluxStageRankingResponseItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "team-stage-ranking";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	int Rank;
	UPROPERTY()
	FString Time;
	UPROPERTY();
	FString Gap;
	UPROPERTY()
	FString TimeSwim;
	UPROPERTY();
	FString TimeTransition;
	UPROPERTY()
	FString TimeRun;
	UPROPERTY();
	FString TimeStart;
	UPROPERTY()
	FString SpeedSwim;
	UPROPERTY()
	FString SpeedRunning;
	UPROPERTY()
	FString SpeedTotal;
};


USTRUCT()
struct DTFLUXAPI_API FDTFluxStageRankingResponse
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

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxRaceDataResponse
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	// ReSharper disable once StringLiteralTypo
	FString Type = "race-datas";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxContestResponse> Datas;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxStatusTeamUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "status-team-update";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	int Status;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxStatusUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "status-update";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	FDTFluxStatusTeamUpdateResponse Datas;
	
};



USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitSensorItemResponse
{
	GENERATED_BODY()
	UPROPERTY()
	int Bib;
	UPROPERTY()
	FString Type = "split-sensor-item";
	UPROPERTY()
	int ContestID;
	UPROPERTY()
	int StageID;
	UPROPERTY()
	int SplitID;
	UPROPERTY()
	FString Time = "-";
	UPROPERTY()
	FString Gap = "-";
	UPROPERTY()
	int Rank;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitSensorResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "split-sensor";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxSplitSensorItemResponse> Datas;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxTeamListItemResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "team-list-item";
	UPROPERTY()
	int ContestId;
	UPROPERTY()
	int Bib;
	UPROPERTY()
	FString FirstName;
	UPROPERTY()
	FString LastName;
	UPROPERTY()
	FString FirstName2 = "";
	UPROPERTY()
	FString LastName2 = "";
	UPROPERTY()
	FString Team = "";
	UPROPERTY()
	FString Gender;
	UPROPERTY()
	FString Gender2;
	UPROPERTY()
	bool Elite;
	UPROPERTY()
	FString Category;
	UPROPERTY()
	int Status;
	UPROPERTY()
	FString Club;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxTeamListResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "team-list";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxTeamListItemResponse> Datas;
	
};


USTRUCT()
struct DTFLUXAPI_API FDTFluxTeamUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "team-update";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxTeamListItemResponse> Datas;
	
};


