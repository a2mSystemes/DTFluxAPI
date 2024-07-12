// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxModel/DTFluxModel.h"
#include "UObject/Object.h"
#include "DTFluxDataStorage.generated.h"

/**
 * 
 */
struct FDTFluxStageRanking;
struct FDTFluxTeam;
struct FDTFluxParticipant;
struct FDTFluxStage;
struct FDTFluxContest;

UENUM(BlueprintType, Category="DTFlux|DataStorage")
// ReSharper disable once IdentifierTypo
enum EDTFluxDataStorageEventType : uint8
{
	UnknownEvent = 0 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantCreateEvent = 1 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantUpdateEvent = 2 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantDeleteEvent = 3 UMETA(DisplayName="ParticipantUpdateEvent"),
	ParticipantStatusUpdateEvent = 4 UMETA(DisplayName="ParticipantUpdateEvent"),
	RaceDataCreateEvent = 5 UMETA(DisplayName="ParticipantUpdateEvent"),
	RaceDataUpdateEvent = 6 UMETA(DisplayName="ParticipantUpdateEvent"),
	RaceDataDeleteEvent = 7 UMETA(DisplayName="ParticipantUpdateEvent"),
	ContestRankingUpdate = 8 UMETA(DisplayName="ParticipantUpdateEvent"),
	StageRankingUpdate = 9 UMETA(DisplayName="ParticipantUpdateEvent"),
	SplitRankingUpdate = 10 UMETA(DisplayName="ParticipantUpdateEvent"),
};



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataStorageUpdated, FString, What);

UCLASS(BlueprintType, Category="DTFlux|Datastorage")
class DTFLUXAPI_API UDTFluxDataStorage : public UObject
{
 GENERATED_BODY()
	friend FDTFluxContest;
	friend FDTFluxStage;

public:

 UPROPERTY(BlueprintReadOnly, Category="DTFlux|DataStorage")
	TArray<FDTFluxContest> Contests;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|DataStorage")
	int CurrentStageId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|DataStorage")
	int CurrentContestId = 0;
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	int CurrentContest()
	{
		if(IsInitialized())
		{
			return CurrentContestId;
		}
		return -1;
	}

	void UpdateSplitRanking(const FDTFluxStageRankingResponse& SplitRankingResponse);
	// void UpdateParticipant(const FDTFluxTeamUpdateResponse& TeamUpdateResponse);
	void UpdateParticipantStatus(const FDTFluxStatusUpdateResponse& StatusUpdateResponse);
	bool IsFinisherSplit(const FDTFluxSplitSensorResponse& SplitSensorResponse);
	FDTFluxFinisher GetFinisherStatus(const FDTFluxSplitSensorResponse& SplitSensorResponse);

	
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	bool GetContest(FDTFluxContest& OutContest, const int& ContestId);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxStage> GetStages(const int ContestId);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	bool GetStage( FDTFluxStage& Stage,const int& StageId = -1);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxParticipant> GetParticipants(const int ContestId = -1);
	UFUNCTION()
	void GetParticipant(const int ContestID, const int ParticipantBib, FDTFluxParticipant& OutParticipant);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxStageRanking> GetStageRanking(const int ContestId, const int StageId);

	UFUNCTION(BlueprintType, Category="DTFlux|Datastorage")
	void AddOrUpdateContest(const FDTFluxContestResponse& ContestResponse);
	UFUNCTION(BlueprintType, Category="DTFlux|Datastorage")
	void AddOrUpdateParticipant(const FDTFluxTeamListItemResponse& TeamListItemResponse);
	UFUNCTION(BlueprintType, Category="DTFlux|Datastorage")
	void UpdateContestRanking(const FDTFluxContestRankingResponse& InContestRanking);
	UFUNCTION(BlueprintType, Category="DTFlux|Datastorage")
	void UpdateStageRanking(const FDTFluxStageRankingResponse& StageRankingResponse);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux")
	bool IsInitialized()
	{
		return Contests.Num() < 0;
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Datastorage")
	bool GetParticipantByBib(const int Bib, FDTFluxParticipant& OutParticipant)
	{
		for(auto& Contest : Contests)
		{
			if(Contest.GetParticipant(Bib, OutParticipant))
			{
				return true;
			}
		}
		return false;
	}


	UFUNCTION(BlueprintCallable, Category="DTFlux")
	void AddSplitSensorResult(FDTFluxSplitSensorItemResponse Response);
	UFUNCTION(BlueprintCallable, Category="DTFlux")
	void ResetStageId(){ CurrentStageId = 0; }
	UFUNCTION(BlueprintCallable, Category="DTFlux")
	void SetCurrentStage(int NewId){ CurrentStageId = NewId; }
	UFUNCTION(BlueprintCallable, Category="DTFlux")
	void GoToNextStage();
	UFUNCTION(BlueprintCallable, Category="DTFlux")
	void ChangeCurrentContest();
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	const FString GetConcurrentFormatedName( int Bib, bool Truncate = true, int MaxSize = 20);


};
