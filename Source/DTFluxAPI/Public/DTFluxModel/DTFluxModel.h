// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxAPILog.h"
#include "UObject/Object.h"
#include "DTFluxModel.generated.h"


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxPerson
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString FirstName;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString LastName;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Gender;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString FunctionLine1 = TEXT("");
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString FunctionLine2 = TEXT("");
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxParticipant
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	int Bib = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FDTFluxPerson Person1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FDTFluxPerson Person2;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Category;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Club;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	bool Elite;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Status;
	bool IsTeam()
	{
		return (Person2.FirstName != "");
	}
};



USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxContestRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxParticipant Participant;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Time;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxStageRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxParticipant Participant;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeSwim;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeTransition;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeRun;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString TimeStart;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitGapItem
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxParticipant Participant;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Gap;
};
USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitGap
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	TArray<FDTFluxSplitGapItem> SplitGapItems;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplit
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Id = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Name;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	TArray<FDTFluxSplitGap> SplitGaps;

	void Dump() const;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxStage
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	int Id;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Name;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FDateTime StartTime;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FDateTime EndTime;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	TArray<FDTFluxSplit> Splits;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	TArray<FDTFluxStageRanking> StageRanking;
	bool SetStartTime(const FDateTime& ContestDate, const FString& TimeString);
	bool SetEndTime(const FDateTime& ContestDate, const FString& TimeString);
	bool UpdateStageRanking(TArray<TSharedPtr<FJsonValue>> StageRankingData);
	bool AddSplit(TArray<TSharedPtr<FJsonValue>> SplitData);
	void Dump() const;

protected:
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxContest
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	int Id = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Name;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	TArray<FDTFluxParticipant> Participants;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	TArray<FDTFluxStage> Stages;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FDateTime Date;

	bool AddStage( const TArray<TSharedPtr<FJsonValue>> StagesData, const TArray<FDTFluxSplit> Splits);
	bool SetDate(const FString& StringDate);
	void AddParticipant(const FDTFluxParticipant& Participant)
	{
		if(Participants.IsEmpty())
		{
			Participants.Add(Participant);
			return;
		}
		int Index = 0;
		FDTFluxParticipant ToUpdate;
		bool Update = false;
		for(auto P : Participants)
		{
			if(P.Bib == Participant.Bib)
			{
				ToUpdate = P;
				Update = true;
				break;
			}
			else
			{
				Index++;
			}
		}
		if(Update)
		{
			Participants.RemoveAt(Index);
		}
		Participants.Add(Participant);
	};

	bool GetParticipant(const int Bib, FDTFluxParticipant& OutParticipant)
	{
		for (auto& Participant : Participants)
		{
			if(Participant.Bib == Bib)
			{
				OutParticipant = Participant;
				return true;
			}
		}
		return false;
	}
	
	void DumpParticipant()
	{
		int Num = 0;
		for(const auto& Participant: Participants )
		{
			UE_LOG(LogDTFluxAPI, Log, TEXT("DUMP Participant : Name -> %s Bib %d"), *Participant.Person1.FirstName, Participant.Bib);
			Num ++;
		}
		UE_LOG(LogDTFluxAPI, Log, TEXT("DUMP Participant : In Contest with ID %d there are %d Participant(s)"), Id, Num);

	};
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitSensorItemResponse
{
	GENERATED_BODY()
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
	FString Time;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitSensorResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "split-sensor";
	UPROPERTY()
	TArray<FDTFluxSplitSensorItemResponse> Datas;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxTeamListItemResponse
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
	bool Elite;
	UPROPERTY()
	FString Category;
	UPROPERTY()
	FString Status;
	UPROPERTY()
	FString Club;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxTeamListResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "team-list";
	UPROPERTY()
	TArray<FDTFluxTeamListItemResponse> Datas;
	
};


USTRUCT()
struct DTFLUXAPI_API FDTFluxTeamUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "team-update";
	UPROPERTY()
	TArray<FDTFluxTeamListItemResponse> Datas;
	
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FStageResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "stage-response-data";
	UPROPERTY()
	int Id;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString StartTime;
	UPROPERTY()
	FString EndTime;
};


USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FSplitResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "split-response-data";
	UPROPERTY()
	int Id;
	UPROPERTY()
	FString Name;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxContestResponse
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FString Type = "contest";
	UPROPERTY()
	int Id;
	UPROPERTY()
	FString Name;
	UPROPERTY()
	FString Date;
	UPROPERTY()
	TArray<FStageResponse> Stages;
	UPROPERTY()
	TArray<FSplitResponse> Splits;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxTeamContestRankingResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "team-contest-ranking";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	int Rank;
	UPROPERTY()
	FString Time;
	UPROPERTY();
	FString Gap;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxContestRankingResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "contest-ranking";
	UPROPERTY()
	int ContestID;
	UPROPERTY()
	TArray<FDTFluxTeamContestRankingResponse> Datas;
};


USTRUCT()
struct DTFLUXAPI_API FDTFluxTeamStageRankingResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "team-stage-ranking";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	int Rank;
	UPROPERTY()
	FString Time;
	UPROPERTY();
	FString Gap;
	UPROPERTY()
	FString TimeSwim;
	UPROPERTY();
	FString TimeTransition;
	UPROPERTY()
	FString TimeRun;
	UPROPERTY();
	FString TimeStart;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxStageRankingResponse
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString Type = "stage-ranking";
	UPROPERTY()
	int ContestID;
	UPROPERTY()
	int StageID;
	UPROPERTY()
	int SplitID;
	UPROPERTY()
	TArray<FDTFluxTeamStageRankingResponse> Datas;
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxRaceDataResponse
{
	GENERATED_BODY()
public:
	
	UPROPERTY()
	FString Type = "race-datas";
	UPROPERTY()
	TArray<FDTFluxContestResponse> Datas;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxStatusTeamUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "status-team-update";
	UPROPERTY()
	int Bib;
	UPROPERTY()
	FString Status;
};

USTRUCT()
struct DTFLUXAPI_API FDTFluxStatusUpdateResponse
{
	GENERATED_BODY()
	UPROPERTY()
	FString Type = "status-update";
	TArray<FDTFluxStatusTeamUpdateResponse> Datas;
	
};

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


