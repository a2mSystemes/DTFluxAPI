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


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDataStorageUpdated, FString, What);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataStorageInit);

UCLASS(BlueprintType, Category="DTFlux|Datastorage")
class DTFLUXAPI_API UDTFluxDataStorage : public UObject
{
 GENERATED_BODY()
	friend FDTFluxContest;
	friend FDTFluxStage;

public:

	UPROPERTY(BlueprintAssignable, Category="DTFlux|DataStorage|Event")
	FOnDataStorageInit OnDataStorageInit;
	UPROPERTY(BlueprintAssignable, Category="DTFlux|DataStorage|Event")
	FOnDataStorageUpdated OnDataStorageUpdated;
	
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
	bool GetContest(const int ContestId, FDTFluxContest& OutContest);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxStage> GetStages(const int ContestId);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	bool GetStage( const int ContestId, const int StageId, FDTFluxStage& OutStage);
	
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
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxStageRanking> GetPoursuitWithStageTime(const TArray<int> ContestIds, const int StageId, float DelaTimeSeconds = 300.0f);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxStageRanking> GetPoursuitWithTimeStart(const TArray<int> ContestIds, const int StageId, float DelaTimeSeconds);

	UFUNCTION()
	bool GetFirstStageOfContest(const int ContestId, FDTFluxStage& Stage);
	void DumpContest();
	UFUNCTION()
	bool GetSplit(const int ContestID, const int StageID, const int SplitID, FDTFluxSplit& OutSplit);
	UFUNCTION()
	FDTFluxSplitRanking AddSplitRanking(const FDTFluxSplitSensorItemResponse& SplitSensorItem);
	UFUNCTION()
	EDTFluxSplitType GetSplitStatus(int ContestID, int StageID, int SplitID);
	
	bool GetStageRankingForBib(int ContestID, int StageID, int Bib, FDTFluxStageRanking& OutStageRanking);
};
