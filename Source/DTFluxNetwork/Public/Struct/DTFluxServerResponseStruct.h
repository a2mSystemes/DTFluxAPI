// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxServerResponseStruct.generated.h"




/**
 * Struct representing a mixed root json server response
 */
USTRUCT()
struct DTFLUXNETWORK_API FDTFluxServerResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "";
	UPROPERTY()
	int Code = -1;
	UPROPERTY()
	FString Message = "";
	UPROPERTY()
	FString Trigger = "";
	UPROPERTY()
	int ContestID = -1;
	UPROPERTY()
	int StageID = -1;
	UPROPERTY()
	int SplitID = -1;
	UPROPERTY()
	FDateTime ReceivedAt;
	UPROPERTY()
	FString RawMessage;
	UPROPERTY()
	FName RequestId = FName("");
	UPROPERTY()
	FText FailureReason;
	
};










