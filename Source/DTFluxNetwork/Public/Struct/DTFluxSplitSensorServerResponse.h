// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxSplitSensorServerResponse.generated.h"

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxSplitSensorItemResponse
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	int Bib;
	UPROPERTY()
	FString Type = "split-sensor-item";
	UPROPERTY()
	int ContestID;
	UPROPERTY()
	int StageID;
	UPROPERTY()
	int SplitID;
	UPROPERTY()
	FString Time = "-";
	UPROPERTY()
	FString Gap = "-";
	UPROPERTY()
	int Rank;
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxSplitSensorResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "split-sensor";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxSplitSensorItemResponse> Datas;
};
