// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxModel/DTFluxModel.h"
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
};
