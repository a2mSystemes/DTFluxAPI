// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/EngineSubsystem.h"
#include "Types/Enum/DTfluxCoreEnum.h"
#include "Types/Struct/DTFluxRaceDataStructs.h"
#include "Types/Struct/DTFluxTeamListStruct.h"
#include "Types/Struct/DTFluxRankingStructs.h"
#include "DTFluxCoreSubsystem.generated.h"




class UFDTFluxNetworkSubsystem;
/** Forward Decl */
class UDTFluxModelAsset;

/**
 * 
 */
UCLASS()
class DTFLUXCORESUBSYSTEM_API UDTFluxCoreSubsystem : public UEngineSubsystem
{
	GENERATED_BODY()

	
public:


	// TSharedPtr<FDTFluxParser> Parser;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSplitRankings, FDateTime, ReceivedAt, TArray<FDTFluxSplitRanking>, SplitRankings);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Core Subsystem")
	FOnSplitRankings OnSplitRankings;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStageRankings, FDateTime, ReceivedAt, TArray<FDTFluxStageRanking>, StageRankings);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Core Subsystem")
	FOnStageRankings OnStageRankings;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnContestRankings, FDateTime, ReceivedAt, TArray<FDTFluxContestRanking>, ContestRankings);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Core Subsystem")
	FOnContestRankings OnContestRankings;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamList, FDateTime, ReceivedAt, TArray<FDTFluxParticipant>, TeamList);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Core Subsystem")
	FOnTeamList OnTeamList;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamUpdate, FDateTime, ReceivedAt, FDTFluxParticipant, TeamUpdatedList);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Core Subsystem")
	FOnTeamUpdate OnTeamUpdate;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTeamStatusUpdate, FDateTime, ReceivedAt, FDTFluxParticipant, TeamUpdated);
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Core Subsystem")
	FOnTeamStatusUpdate OnTeamStatusUpdate;
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void SendTeamListRequest();	
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void SendRaceDataRequest();	
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void SendContestRankingRequest(int InContestId);	
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void SendStageRankingRequest(int InContestId, int InStageId, bool bShouldIncludeSplitRanking = true);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void RequestAllStageRankingOfContest(int InContestId, int InStageId, bool bShouldIncludeSplitRanking = true);	
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void SendSplitRankingRequest(int InContestId, int InStageId, int InSplitId);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void RequestAllSplitRankingOfContest(int InContestId, int InStageId);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void RequestAllSplitRankingOfStage(int InContestId, int InStageId, int InSplitId);

	UFUNCTION(BlueprintCallable, Category="DTFlux|Core Subsystem")
	void RefreshStorage();


	
protected:
	// ~Subsystem Interface
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// ~Subsystem Interface
private:
	UFDTFluxNetworkSubsystem* NetworkSubsystem = nullptr;

	UFUNCTION()
	void ParseRaceData(const FDTFluxRaceData& RaceDataDefinition);
	UFUNCTION()
	void ParseTeamList(const FDTFluxTeamListDefinition& TeamListDefinition);
	UFUNCTION()
	void OnDataReceived();
	UFUNCTION()
	void SendRequest(const FString& Message);
	UFUNCTION()
	void RegisterDelegates();

	UPROPERTY()
	UDTFluxModelAsset* DataStorage = nullptr;
};
