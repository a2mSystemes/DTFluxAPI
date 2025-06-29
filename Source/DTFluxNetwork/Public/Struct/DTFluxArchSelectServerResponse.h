// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxArchSelectServerResponse.generated.h"

USTRUCT(BlueprintType)
struct  FDTFluxArchSelectResponseItem
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "arch-select";
	UPROPERTY()
	int ContestId;
	UPROPERTY()
	int StageId;
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxArchSelectResponse
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString Type = "arch-select";
	UPROPERTY()
	TArray<FDTFluxArchSelectResponseItem> Datas;
};
