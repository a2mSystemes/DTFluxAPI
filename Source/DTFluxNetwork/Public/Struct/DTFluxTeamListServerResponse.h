// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/Struct/DTFluxTeamListStruct.h"
#include "UObject/Object.h"
#include "DTFluxTeamListServerResponse.generated.h"

/**
 * @struct FDTFluxTeamUpdateResponse
 * Struct representing the Participant List Update Response from the server
 * Used to exchange an update of a list of participant to the server
 */
USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxTeamUpdateResponse 
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "team-update";
	UPROPERTY()
	int Bib = -1;
	UPROPERTY()
	FString Status;
};

/**
 *	@struct FDTFluxTeamListResponse
 *	Struct representing the Participant List Response from the server
 *	Used to exchange data between the system and the API
 */
USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxTeamListResponse : public FDTFluxTeamListDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "team-list";
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxStatusTeamUpdateResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "status-team-update";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	int Status;
};

USTRUCT(BlueprintType)
struct DTFLUXNETWORK_API FDTFluxStatusUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "status-update";
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	FDTFluxStatusTeamUpdateResponse Datas;
	
};