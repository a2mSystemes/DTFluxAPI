// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CrossCompilerCommon.h"
#include "DTFluxModel/DTFluxModel.h"
#include "DTFluxSubsystem/DTFluxSubsystem.h"
#include "UObject/Object.h"
#include "DTFluxUtils.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Category="DTFlux|Model|Helpers")
class DTFLUXAPI_API UDTFluxModelHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="DTFlux|Model")
	static bool IsParticipantATeam(const FDTFluxParticipant& Participant)
	{
		return Participant.Person2.FirstName != "";
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static TArray<FDTFluxSplitRanking> GetSplitRanking(const int ContestId, const int StageId,
		const int SplitId, const int From = 0, const int DisplayNumber = 0)
	{
		TArray<FDTFluxSplitRanking> SplitRankings;
		UDTFluxSubsystem* Subsystem = GEngine->GetEngineSubsystem<UDTFluxSubsystem>();
		TArray<FDTFluxContest> Contests = Subsystem->GetDataStorage()->Contests;
		for( auto& Contest : Contests)
		{
			if(Contest.Id == ContestId)
			{
				for( auto& Stage : Contest.Stages)
				{
					if(Stage.Id == StageId)
					{
						for( auto& Split : Stage.Splits)
						{
							if(Split.Id == SplitId)
							{
								Split.SortByRank();
								return Split.GetSplitRanking(From, DisplayNumber);
							}
						}
					}
				}
			}
		}
		return SplitRankings;
		
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static TArray<FDTFluxStageRanking> GetStageRanking(const int ContestId, const int StageId, const int From = 0, const int DisplayNumber = 0)
	{
		TArray<FDTFluxStageRanking> StageRankings;
		UDTFluxSubsystem* Subsystem = GEngine->GetEngineSubsystem<UDTFluxSubsystem>();
		TArray<FDTFluxContest> Contests = Subsystem->GetDataStorage()->Contests;
		for( auto& Contest : Contests)
		{
			if(Contest.Id == ContestId)
			{
				for( auto& Stage : Contest.Stages)
				{
					if(Stage.Id == StageId)
					{
						StageRankings = Stage.StageRanking;
					}
				}
			}
		}
		//CAREFUL Can Be Empty
		return StageRankings;
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static TArray<FDTFluxContestRanking> GetContestRanking(const int ContestId, const int StageId, const int From = 0, const int DisplayNumber = 0)
	{
		TArray<FDTFluxContestRanking> ContestRankings;
		UDTFluxSubsystem* Subsystem = GEngine->GetEngineSubsystem<UDTFluxSubsystem>();
		TArray<FDTFluxContest> Contests = Subsystem->GetDataStorage()->Contests;
		for( auto& Contest : Contests)
		{
			if(Contest.Id == ContestId)
			{
				ContestRankings = Contest.ContestRanking;
			}
		}
		//CAREFUL Can Be Empty
		return ContestRankings;
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static bool GetParticipant(const int Bib, FDTFluxParticipant& Participant)
	{
		UDTFluxSubsystem* Subsystem= GEngine->GetEngineSubsystem<UDTFluxSubsystem>();
		UDTFluxDataStorage* DataStorage = Subsystem->GetDataStorage();

		return DataStorage->GetParticipantByBib(Bib, Participant);
	
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static FString GetParticipantString(const int Bib, bool Truncate = true, int MaxSize = 20)
	{
		FString ParticipantStr = "";
		FDTFluxParticipant Participant;
		if(UDTFluxModelHelper::GetParticipant(Bib, Participant))
		{
			ParticipantStr = Participant.GetParticipantFormatedName(Truncate, MaxSize);
		}
		return ParticipantStr;
	}

	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static EDTFluxStageStatusType GetStatusType(const int ContestID, const int StageID, UDTFluxDataStorage* DataStorage);


	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static int GetCurrentContest(UDTFluxDataStorage* DataStorage);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static TArray<int> GetCurrentStage(UDTFluxDataStorage* DataStorage);

		UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static FString GetCurrentStageName(UDTFluxDataStorage* DataStorage);
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Model|Helpers")
	static void SortByRank(TArray<FDTFluxSplitRanking>& SplitRankingArray);
	
};
