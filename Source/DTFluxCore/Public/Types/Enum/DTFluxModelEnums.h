// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxModelEnums.generated.h"

/**
 * 
 */
UENUM(BlueprintType, Category="DTFlux|Model")
enum class EDTFluxParticipantStatusType : uint8
{
 Normal = 0 UMETA(DisplayName="Normal"),
 OutOfRace = 1 UMETA(DisplayName="HorsCourse"),
 DSQ = 2 UMETA(DisplayName="Disqualifié"),
 DNF = 3 UMETA(DisplayName="Abandon"),
 DNS = 4 UMETA(DisplayName="NonPartant"),
 NotLinedUp = 5 UMETA(DisplayName="NonPresentAuDépart"),
};


UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor=true))
enum class EParticipantSortingType : uint8
{

	None				= 0 << 1	UMETA(DisplayName="Normal"),
	Alpha				= 1	<< 1	UMETA(DisplayName="Aplha"),
	PoursuiteStartTime  = 1	<< 2	UMETA(DisplayName="Poursuite StartTime"),
	Rank				= 1	<< 3	UMETA(DisplayName="Rank"),
	IgnoreEmpty			= 1 << 4	UMETA(DisplayName="IgnoreEmpty"),
};
ENUM_CLASS_FLAGS(EParticipantSortingType);


UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor=true))
enum class EDTFluxFinisherType : uint8
{
	None	= 0b0000000		UMETA(DisplayName="Unknown"),
	Finish	= 0b0000001		UMETA(DisplayName="Finish"),
	Winner	= 0b0000010		UMETA(DisplayName="Winner"),
	Spotter = 0b0000100		 UMETA(DisplayName="Spotter"),
};
ENUM_CLASS_FLAGS(EDTFluxFinisherType);

UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor=true))
enum class EDTFluxSplitType : uint8
{
	None			= 0b00000000		UMETA(DisplayName="Undefined"),
	PreFinnish		= 0b00000001		UMETA(DisplayName="PreFinnishSplit"),
	Finish			= 0b00000010		UMETA(DisplayName="FinishSplit"),
	Regular			= 0b00000100		 UMETA(DisplayName="Regular"),
};
ENUM_CLASS_FLAGS(EDTFluxSplitType);


UENUM(BlueprintType, meta=(Bitflags, UseEnumValuesAsMaskValuesInEditor=true))
enum EDTFluxSortingFilter  : uint8
{
	None			= 0b00000000				UMETA(DisplayName="No Sorting"),
	IgnoreStatusOut	= 0b00000001				UMETA(DisplayName="IgnoreStatusOut"),
	Ascending		= 0b00000010				UMETA(DisplayName="Ascending"),
	Descending		= 0b00000100				UMETA(DisplayName="Descending"),
	IgnoreEmpty		= 0b00001000				UMETA(DisplayName="IgnoreEmpty"),
	ByRank			= 0b00010000				UMETA(DisplayName="ByRank"),
	ByAlpha			= 0b01000000				UMETA(DisplayName="ByAlpha"),
	ByStartTime		= 0b00100000				UMETA(DisplayName="ByStartTime"),
	AscendingByRank = Ascending | ByRank		UMETA(DisplayName="AscendingByRank"),	
	DescendingByRank= Descending | ByRank		UMETA(DisplayName="DescendingByRank")	
};
ENUM_CLASS_FLAGS(EDTFluxSortingFilter);
