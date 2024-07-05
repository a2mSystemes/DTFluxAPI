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

UCLASS(BlueprintType, Category="DTFlux|Datastorage")
class DTFLUXAPI_API UDTFluxDataStorage : public UObject
{
 GENERATED_BODY()
	friend FDTFluxContest;
	friend FDTFluxStage;

public:
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	bool UpdateDataStorage(const FString JsonPayload);
	
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
	}; 
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	bool GetContest(FDTFluxContest& OutContest, const int& ContestId);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxStage> GetStages(const int ContestId);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	bool GetStage( FDTFluxStage& Stage,const int& StageId = -1);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxParticipant> GetParticipants(const int ContestId = -1);
	UFUNCTION()
	FDTFluxParticipant GetParticipant(const int ContestID, const int ParticipantBib);
	UFUNCTION(BlueprintCallable, Category="DTFlux|DataStorage")
	TArray<FDTFluxStageRanking> GetStageRanking(const int ContestId, const int StageId);

	UFUNCTION(BlueprintType, Category="DTFlux|Datastorage")
	void AddOrUpdateContest(const FDTFluxContestResponse& ContestResponse);
	UFUNCTION(BlueprintType, Category="DTFlux|Datastorage")
	void AddOrUpdateParticipant(const FDTFluxTeamListItemResponse& TeamListItemResponse);
	
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

};
