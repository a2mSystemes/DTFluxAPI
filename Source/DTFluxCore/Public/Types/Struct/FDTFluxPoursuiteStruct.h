// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxTeamListStruct.h"
#include "UObject/Object.h"
#include "FDTFluxPoursuiteStruct.generated.h"


/**
 * @struct FDTFluxPoursuite
 * Representing a 
 */
USTRUCT(BlueprintType, Category="DTFlux|Poursuite")
struct FDTFluxPoursuite
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Poursuite")
	FDTFluxParticipant Participant;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Poursuite")
	FDateTime TimeStart;
	FText GetParticipantFormatedName() const;

};

