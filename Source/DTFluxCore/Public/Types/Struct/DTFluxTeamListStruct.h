// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxCoreModule.h"
#include "Types/Enum/DTFluxModelEnums.h"
#include "DTFluxTeamListStruct.generated.h"

USTRUCT()
struct DTFLUXCORE_API FDTFluxTeamListItemDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "team-list-item";
	UPROPERTY()
	int ContestId;
	UPROPERTY()
	int Bib;
	UPROPERTY()
	FString FirstName;
	UPROPERTY()
	FString LastName;
	UPROPERTY()
	FString FirstName2 = "";
	UPROPERTY()
	FString LastName2 = "";
	UPROPERTY()
	FString Team = "";
	UPROPERTY()
	FString Gender;
	UPROPERTY()
	FString Gender2;
	UPROPERTY()
	bool Elite;
	UPROPERTY()
	FString Category;
	UPROPERTY()
	int Status;
	UPROPERTY()
	FString Club;
};

/**
 *	@struct FDTFluxTeamListDefinition
 *	Struct representing the Participant List definition
 * Used to exchange data between Objects in the system
 */
USTRUCT(BlueprintType)
struct DTFLUXCORE_API FDTFluxTeamListDefinition
{
	GENERATED_BODY()
public:
	UPROPERTY()
	// ReSharper disable once IdentifierTypo
	TArray<FDTFluxTeamListItemDefinition> Datas;	
};



USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXCORE_API FDTFluxPerson
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString FirstName;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString LastName;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString Gender;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString FunctionLine1 = TEXT("");
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString FunctionLine2 = TEXT("");

	bool operator==(const FDTFluxPerson& Right) const
	{
		return FirstName.ToLower() + LastName.ToLower() + Gender.ToLower()
				== Right.FirstName.ToLower() + Right.LastName.ToLower() + Right.Gender.ToLower();
	}
	bool operator==(const int Length) const
	{
		return (FirstName.ToLower() + LastName.ToLower() + Gender.ToLower()).Len() == Length;
	}
	bool operator!=(const int Length) const
	{
		return !(*this == Length);
	}
	bool operator!=(const FDTFluxPerson& Right) const
	{
		return FirstName.ToLower() + LastName.ToLower() + Gender.ToLower()
				!= Right.FirstName.ToLower() + Right.LastName.ToLower() + Right.Gender.ToLower();
	}
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXCORE_API FDTFluxParticipant
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	int Bib = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FDTFluxPerson Person1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FDTFluxPerson Person2;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString Category;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString Club;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	bool Elite;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	EDTFluxParticipantStatusType Status;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	FString Team;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model", EditAnywhere)
	bool bIsMassStartParticipant = false;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	int LastSplitId = 0;
	bool IsTeam() const;
	void Dump() const;
	FString GetParticipantFormatedName(bool Truncate = false, int MaxSize = 20) const;
	
};