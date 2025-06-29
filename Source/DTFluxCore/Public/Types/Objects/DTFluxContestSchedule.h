// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxContestSchedule.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DTFLUXCORE_API UDTFluxParticipantSchedule : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LastName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString FirstName2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LastName2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Bib;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Elite = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Club;
};




/**
 * 
 */
UCLASS(Blueprintable)
class DTFLUXCORE_API UDTFluxStageSchedule : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 StageId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StageName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime StartTime;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime EndTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime CutOff;

};

/**
 * 
 */
UCLASS(Blueprintable)
class DTFLUXCORE_API UDTFluxContestSchedule : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ContestId;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDateTime Date;


	
};
