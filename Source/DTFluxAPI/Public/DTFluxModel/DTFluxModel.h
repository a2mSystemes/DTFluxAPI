// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DTFluxModel.generated.h"


// Forward declarations
struct FDTFluxTeam;

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFLuxStartStageData 
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Type;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Contest;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Stage;

	// Maybe this one can be made DATETIME
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString RealStartTime;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxStartStagePayload
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Description;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Trigger;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Type;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FDTFLuxStartStageData> Datas;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxStartListItemPayload
{
	GENERATED_BODY()

	// "Report": "Start List",
	// "ContestID": 0,
	// "ContestName": "",
	// "Bib": 1039,
	// "Firstname": "Mystère",
	// "Lastname": "Mystère",
	// "Gender": "",
	// "Club": "",
	// "Firstname2": "",
	// "Lastname2": "",
	// "Gender2": "",
	// "Club2": "",
	// "TeamName": "",
	// "Category": "",
	// "Elite": false
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int ContestID;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString ContestName;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Bib;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString FirstName;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString LastName;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Gender;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Club;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString FirstName2;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString LastName2;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Gender2;
		
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Club2;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString TeamName;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Category;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	bool Elite;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxStartListPayload
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FDTFluxStartListItemPayload> Participants;
	
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxParticipant
{
	GENERATED_BODY()

public:
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString FirstName;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString LastName;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Club;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Gender;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Category;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxTeam
{
	GENERATED_BODY()

public:
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FDTFluxParticipant> Participants;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString TeamName;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Bib;	
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FDTFluxContest
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int ContestID;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString ContestName;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FDTFluxTeam> TeamParticipants;

	bool AddTeam(FDTFluxTeam Team)
	{
		TeamParticipants.Add(Team);
		return true;
	}
	bool TeamAlreadyExist(FDTFluxTeam Team)
	{
		for(const auto& MyTeam : TeamParticipants)
		{
			// Compare Bib
			if(MyTeam.Bib == Team.Bib )
			{
				return true;
			}
		}
		return false;
	}
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxContestList
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TMap<FString, FDTFluxContest>Contests;

	int GetContestId(const FString& ContestName) const
	{
		const FDTFluxContest* Contest = Contests.Find(ContestName);
		if(Contest)
		{
			return Contest->ContestID;
		}
		return -1;
	}
	FString GetContestName(const int& ContestId) const
	{
		for (const auto& Contest : Contests)
		{
			if (Contest.Value.ContestID == ContestId)
			{
				return Contest.Key;
			}
		}
		return FString();
	}
};


UCLASS(Blueprintable, Category="DTFlux|Model")
class DTFLUXAPI_API UDTFluxContestBPFn : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "DT Http Server|Params")
	static int GetContestId(const FDTFluxContestList& ContestList, const FString& ContestName)
	{
		return ContestList.GetContestId(ContestName);
	}
	UFUNCTION(BlueprintPure, Category = "DT Http Server|Params")
	static FString GetContestName(const FDTFluxContestList& ContestList, const int& ContestId)
	{
		return ContestList.GetContestName(ContestId);
	}
};

// TeamListItem Response from proxy Containing Team definition
USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FResponseTeamListItem
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Contest;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Bib;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString FirstName;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString LastName;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Gender;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	bool Elite;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Club;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Team;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Category;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Status;
};
// TeamList Response from proxy Containing List of Team
USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseTeamList
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FResponseTeamListItem> Datas;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseStageListItem
{
	GENERATED_BODY()
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Id;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Name;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString StartTime;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString EndTime;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseSplitsListItem
{
	GENERATED_BODY()


	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Id;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Name;
	
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseRaceListItem
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Id;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Name;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Date;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FProxyResponseStageListItem> Stages;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FProxyResponseSplitsListItem> Splits;
	
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseContestRankingListItem
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Bib;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Rank;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Time;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Gap;
	
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseContestRankingList
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int ContestId;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FProxyResponseContestRankingListItem> Datas;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseStageRankingListItem
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Bib;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int Rank;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Time;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString Gap;
		
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString TimeSwim;
			
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString TimeTransition;
			
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString TimeRun;
			
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	FString TimeStart;
	
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct FProxyResponseStageRankingList
{
	GENERATED_BODY()

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int ContestId;
	
	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	int StageId;

	UPROPERTY(Blueprintable, Category="DTFlux|Model", BlueprintReadOnly)
	TArray<FProxyResponseStageRankingListItem> Datas;

};