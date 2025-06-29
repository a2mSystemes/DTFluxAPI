// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxRequestStructs.generated.h"


/**
 * Struct representing a base json object request to the server
 * Used to request data to the server
 */
USTRUCT()
struct FDTFluxRequestBase
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Path = "";
};

/**
 * Struct representing a RaceData json object request to the server
 * RaceData represents all data concerning the Race and its different Contests, Stages and Splits.
 */
USTRUCT()
struct FDTFluxRaceDataRequest: public FDTFluxRequestBase
{
	GENERATED_BODY()
public:
	FDTFluxRaceDataRequest(){
		Path = "race-datas";
	}
};

/**
 * Struct representing a TeamList json object request to the server
 * TeamList is the list of participants of the events
 */
USTRUCT()
struct FDTFluxTeamListRequest: public FDTFluxRequestBase
{
	GENERATED_BODY()
public:
	FDTFluxTeamListRequest(){
		Path = "team-list";
	}
};

/**
 * Struct representing a Ranking json request object for a specific to the server
 */
USTRUCT()
struct FDTFluxContestRankingRequest: public FDTFluxRequestBase
{
	GENERATED_BODY()
	
public:
	FDTFluxContestRankingRequest()
	{
		Path = "contest-ranking";
		ContestID = -1;
	}
	FDTFluxContestRankingRequest(int InContestID)
	{
		Path = "contest-ranking";
		ContestID = InContestID;
	}

	UPROPERTY()
	int ContestID;
};

/**
 * Struct representing a Ranking json request object for a specific Stage to the server
 */
USTRUCT()
struct FDTFluxStageRankingRequest: public FDTFluxRequestBase
{
	GENERATED_BODY()
	
public:
	FDTFluxStageRankingRequest()
	{
		Path = "stage-ranking";
		ContestID = -1;
		StageID = -1;
		SplitID = -1;
	}
	FDTFluxStageRankingRequest(int InContestID, int InStageId)
	{
		Path = "stage-ranking";
		ContestID = InContestID;
		StageID = InStageId;
		SplitID = -1;
	}

	UPROPERTY()
	int ContestID;
	UPROPERTY()
	int StageID;
	UPROPERTY()
	int SplitID;

	
};

/**
 * Struct representing a Ranking json request object for a specific Split to the server
 */
USTRUCT()
struct FDTFluxSplitRankingRequest: public FDTFluxStageRankingRequest
{
	GENERATED_BODY()
	
public:
	FDTFluxSplitRankingRequest()
	{
		Path = "stage-ranking";
		ContestID = -1;
		StageID = -1;
		SplitID = -1;
	}
	FDTFluxSplitRankingRequest(int InContestID, int InStageId, int InSplitId)
	{
		Path = "stage-ranking";
		ContestID = InContestID;
		StageID = InStageId;
		SplitID = InSplitId;
	}

};
