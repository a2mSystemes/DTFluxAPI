// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "Types/Struct/DTFluxRaceDataStructs.h"
#include "DTFluxModelAsset.generated.h"


class UDTFluxContestStorage;
/**
 * Class representing Data Storage
 */
UCLASS(BlueprintType)
class DTFLUXCORE_API UDTFluxModelAsset : public UObject
{
	GENERATED_BODY()
	UDTFluxModelAsset(const FObjectInitializer& ObjectInitializer);
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString EventName = "MyEvent";


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDTFluxPerson> Persons;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TMap<int /* Bib */, FDTFluxParticipant> Participants;
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FString /* ContestName */, FDTFluxContest> Contests;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<FDTFluxContestRanking> ContestRankings;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TMap<FString /*ContestName*/ ,FDTFluxStageRanking> StageRankings;

	UFUNCTION(BlueprintCallable, CallInEditor, Category="DTFlux|ModelAsset")
	void AddContest(const FDTFluxContest &Contest);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="DTFlux|Contest")
	bool GetContestById(const int InContestId, FDTFluxContest& OutContest);
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category="DTFlux|Person")
	void AddPerson(const FDTFluxPerson& InPerson);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="DTFlux|Participant")
	void AddParticipant(const FDTFluxParticipant& InParticipant, const int ContestId);

	UFUNCTION(BlueprintCallable, CallInEditor, Category="DTFlux|Person")
	bool PersonExists(const FDTFluxPerson& InPerson) const;
};
