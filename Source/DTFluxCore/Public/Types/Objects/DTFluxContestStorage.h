// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Types/Struct/DTFluxRaceDataStructs.h"
#include "DTFluxContestStorage.generated.h"


/**
 * 
 */
UCLASS(BlueprintType)
class DTFLUXCORE_API UDTFluxContestStorage : public UObject
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void Initialize();

	UPROPERTY()
	bool bInitialize = false;

	// UFUNCTION()
	// bool InitializeRaceData(const FDTFluxRaceDataDefinition& InRaceDataDefinition);
	//
	// UFUNCTION()
	// bool InitializeTeamList(const FDTFluxTeamListDefinition& InTeamListDefinition);
	//
	// UFUNCTION()
	// bool UpdateParticipantStatus(const FDTFluxTeamListDefinition& InTeamListDefinition);

#pragma region EventsSection

#pragma endregion
#pragma region ContestRegion
	UPROPERTY()
	TArray<FDTFluxContest> ContestsStore;

	UPROPERTY()
	TMap<int /*Bib*/,FDTFluxContestRanking /*Ranking*/> ContestRankingStore;

	UFUNCTION(BlueprintPure, BlueprintCallable, Category="DTFlux|Contest")
	const FDTFluxContest&  GetCurrentContest() const;

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	const FDTFluxStage& GetCurrentStage(const int InContest, const int InStage);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetContestRankings(const TArray<FDTFluxContestRanking> OutContestRanking, const int InContestId, const bool bShouldSortByRank = true, const bool bShouldSkipEmpty = true);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetContestRankingsSorted( const TArray<FDTFluxContestRanking> OutSortedContestRankings, const int InContestId, const EDTFluxSortingFilter InFilter, const bool bShouldSkipEmpty = true );

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void AddContest(const FDTFluxContest InContest);
	//
	// UFUNCTION(Category="DTFlux|Contest")
	// void AddContestResponse(const FDTFluxContestResponse& InContestResponse);
	
#pragma endregion
#pragma region StageSection
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetStage(const int InContest, const int InStage, const FDTFluxStage& OutStage);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetStages(const int InContest, TArray<FDTFluxStage> OutStages);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetStageCurrentStage(const FDTFluxStage& OutStage);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void AddStage(const int InContest, const FDTFluxStage& InStage);	

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void AddStageRanking(const int InContest, const FDTFluxStageRanking& InStageRanking);

	// UFUNCTION(Category="DTFlux|Contest")
	// void AddStageResponse(const int InContestId, const FDTFluxStageResponse& StageResponse);
	
#pragma endregion 
#pragma region SplitSection
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void AddSplit(const int InContest, const FDTFluxSplit& InSplit);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void AddSplitRanking(const int InContest, const FDTFluxSplitRanking& InSplitRanking);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetSplitRankingByParticipant(const FDTFluxParticipant& InParticipant, const int InContestId, const int InStageId, const FDTFluxSplitRanking& OutSplitRankingForBib);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetSplitRankingByBib(const int InBib, const int InContestId, const int InStageId, const FDTFluxSplitRanking& OutSplitRankingForBib);
	
#pragma endregion 
#pragma region ParticipantSection
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetParticipantsForContest(const int InContestId, const TArray<FDTFluxParticipant> OutParticipants);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	void GetPoursuiteStartupLine(const int InContestId, const TArray<FDTFluxParticipant> OutParticipants);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Contest")
	FDateTime GetMassStart(const int InContestId );
	
#pragma endregion 


protected:
	UFUNCTION()
	void SetCurrentContest();

private:

	UPROPERTY()
	FDTFluxContest CurrentContest;	
	
	UPROPERTY()
	FDTFluxStage CurrentStage;

};
