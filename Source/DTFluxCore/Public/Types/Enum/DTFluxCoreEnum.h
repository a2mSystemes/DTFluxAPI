// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum class EDTFluxRequestType : uint8
{
	None				= 0 UMETA(DisplayName="None"),
	ContestRanking		= 1 UMETA(DisplayName="contest-ranking"),
	StageRanking		= 2 UMETA(DisplayName="stage-ranking"),
	SplitRanking		= 3 UMETA(DisplayName="split-ranking"),
	TeamList			= 4 UMETA(DisplayName="team-list"),
	RaceData			= 5 UMETA(DisplayName="race-data"),
};


UENUM(BlueprintType)
enum class EDTFluxConnectionStatus : uint8
{
	Unset		= 0			UMETA(DisplayName="Unset"),
	Connected	= 1	<< 0	UMETA(DisplayName="Connected"),
	Error		= 2 << 1	UMETA(DisplayName="Error")
};
