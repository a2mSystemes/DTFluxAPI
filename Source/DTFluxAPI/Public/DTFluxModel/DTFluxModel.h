// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxAPILog.h"
#include "DTFluxModelResponse.h"
#include "DTFluxUtils/DTFluxEnums.h"
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
	TEnumAsByte<EDTFluxParticipantStatusType> Status;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	FString Team;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	int LastSplitId = 0;
	bool IsTeam() const;
	void Dump() const;
	FString GetParticipantFormatedName(bool Truncate = false, int MaxSize = 20) const
	{
		FString ParticipantName;
		if(Truncate)
		{
			if(IsTeam())
			{
				//Concatenate the team name;
				if(Team.Len() > MaxSize - 3)
				{
					return Team.Left(MaxSize - 3).Append(TEXT("..."));
				}
				return Team;
			}
			if(Person1.FirstName.Contains("-") )
			{
				FString Formated = "";
				//Compound Firstname
				TArray<FString> Out;
				Person1.FirstName.ParseIntoArray(Out,TEXT("-"),true);
				for(const auto& Str : Out)
				{
					Formated.Append(Str.Left(1).ToUpper()).Append(".");
				}
				// TODO : Camel Case handling for LastName
				Formated.Append(" ").Append(*Person1.LastName);
				UE_LOG(LogDTFluxAPI, Log, TEXT("Firstname is with space compound. Formated Name %s length %02d MAX Size : %02d"),
					*Formated, Formated.Len(), MaxSize);
				if(Formated.Len() >= MaxSize)
				{
					UE_LOG(LogDTFluxAPI, Log, TEXT("Reducing %s Formated"), *Formated);

					return Formated.Left(MaxSize - 3).Append("...");
				}
				return Formated;
			}
			if(Person1.FirstName.Contains(" "))
			{
				FString Formated = "";
				//Compound Firstname
				TArray<FString> Out;
				Person1.FirstName.ParseIntoArray(Out,TEXT(" "),true);
				for(const auto& Str : Out)
				{
					Formated.Append(Str.Left(1).ToUpper()).Append(".");
				}
				// TODO : Camel Case handling for LastName
				Formated.Append(" ").Append(*Person1.LastName);
				UE_LOG(LogDTFluxAPI, Log, TEXT("Firstname is with space compound. Formated Name %s length %02d MAX Size : %02d"),
					*Formated, Formated.Len(), MaxSize);
				if(Formated.Len() >= MaxSize)
				{
					UE_LOG(LogDTFluxAPI, Log, TEXT("Reducing %s Formated"), *Formated);
					return Formated.Left(MaxSize - 3).Append("...");
				}
				return Formated;
			}
			FString Formated = Person1.FirstName.Left(1).Append(". ");
			Formated.Append(Person1.LastName);
			UE_LOG(LogDTFluxAPI, Log, TEXT("Firstname is not compound. Formated Name %s length %02d MAX Size : %02d"),
				*Formated, Formated.Len(), MaxSize);
			if(Formated.Len() >= MaxSize)
			{
				UE_LOG(LogDTFluxAPI, Log, TEXT("Reducing %s Formated"), *Formated);
				return Formated.Left(MaxSize - 3).Append("...");
			}
			return Formated;
		}
		else
		{
			if(!IsTeam())
			{
				return FString::Printf(TEXT("%s %s"), *Person1.FirstName, *Person2.LastName);
			}
			return Team;
		}
	}
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxContestRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Time;
	void Dump()
	{
		UE_LOG(LogDTFluxAPI, Log,
			TEXT("FDTFluxContestRanking ->> \n \"rank\" : %d, Participant with Bib %d \"Gap\" : %s, \"Time\" : %s "),
			Rank, Bib, *Gap, *Time );
	};
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxStageRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib;
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

	void Dump() const
	{	
		UE_LOG(LogDTFluxAPI, Log, TEXT("RANKING : %02d. Participant bib %d %s %s %s %s %s"),
			Rank, Bib, *Gap, *TimeSwim,
			*TimeTransition, *TimeRun, *TimeStart);
	}
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
struct DTFLUXAPI_API FDTFluxSplitRanking
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Gap;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FString Time;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Rank;
	void Dump() const
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("SplitGapItem"))
		// Participant.Dump();
		UE_LOG(LogDTFluxAPI, Log, TEXT("Bib %02d Gap %s"), Bib, *Gap);
	}
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
	TArray<FDTFluxSplitRanking> SplitGaps;

	void Dump() const
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Split %02d::%s *****\n"), Id, *Name);
		for(const auto& SplitGapItem : SplitGaps)
		{
			SplitGapItem.Dump();
		}
	}

	void InsertOrReplace(const FDTFluxStageRankingResponseItem& SplitRankingItemResp)
	{
		FDTFluxSplitRanking NewSplitGapItem;
		NewSplitGapItem.Bib = SplitRankingItemResp.Bib;
		NewSplitGapItem.Gap = SplitRankingItemResp.Gap;
		if(SplitGaps.IsEmpty())
		{
			SplitGaps.Add(NewSplitGapItem);
			return;
		}
		bool Update = true;
		int Idx = 0;
		for(auto& SplitGapItem : SplitGaps)
		{
			if(SplitGapItem.Bib == SplitRankingItemResp.Bib)
			{
				Update = false;
			}
			Idx++;
		}
		if(Update)
		{
			if(SplitGaps.IsValidIndex(Idx))
			{
				SplitGaps.RemoveAt(Idx);
			}
		}
		SplitGaps.Add(NewSplitGapItem);
	};
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
	void Dump() const
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Stage %02d::%s"), Id, *Name);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Starts at %s and is supposed to finnish at %s"),
			*StartTime.ToString(), *EndTime.ToString());
		UE_LOG(LogDTFluxAPI, Log, TEXT("Splits : \n"));
		for(const auto& StageRankingEl : StageRanking)
		{
			StageRankingEl.Dump();
		}
		UE_LOG(LogDTFluxAPI, Log, TEXT("Splits : \n"));
		for(const auto& Split : Splits)
		{
			Split.Dump();
		}

	};
	void SortStageRanking()
	{
		StageRanking.Sort([](const FDTFluxStageRanking& A, const FDTFluxStageRanking& B)
		{
			if(A.Rank == 0 || B.Rank == 0)
				return true;
			return A.Rank > B.Rank;
		});
	};
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
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|model")
	TArray<FDTFluxContestRanking> ContestRanking;

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
				UE_LOG(LogDTFluxAPI, Log, TEXT("Idx %d, OLD : %s %s new %s %s in Contest%02d"),
						Index, *P.Person1.FirstName, *P.Person1.LastName,
						*Participant.Person1.FirstName, *Participant.Person1.LastName, Id);
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
			if(Participants.IsValidIndex(Index))
			{
				UE_LOG(LogDTFluxAPI, Log, TEXT("Idx %d, REMOVED : %s %s in Contest%02d"),
		Index, 
		*Participants[Index].Person1.FirstName, *Participants[Index].Person1.LastName, Id);
				Participants.RemoveAt(Index);	
			}

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
	bool AddContestRanking(const FDTFluxContestRanking& NewRanking)
	{
		bool Update = false;
		if(ContestRanking.IsEmpty())
		{
			ContestRanking.Add(NewRanking);
			return true;
		}
		int Idx = 0;
		for( auto& Ranking : ContestRanking)
		{
			if(NewRanking.Bib == Ranking.Bib)
			{
				// we need to update a ranking
				Update = true;
				break;
			}
			Idx++;
		}
		if(Update)
		{
			ContestRanking.RemoveAt(Idx);
			ContestRanking.Insert(NewRanking, Idx);
			UE_LOG(LogDTFluxAPI, Log,
				TEXT("Inserting %d with rank %d in Contest with ID %d"),
				NewRanking.Bib, NewRanking.Rank, Id );
			return true;
		}
		else
		{
			ContestRanking.Add(NewRanking);
			return true;
		}
		return false;
	}
	void Dump()
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("CONTEST DUMP BEGIN *****%s::%02d *****\n"), *Name, Id);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Date : %s"), *Date.ToString());
		UE_LOG(LogDTFluxAPI, Log, TEXT("PARTICIPANTS : \n"));
		DumpParticipant();
		for(auto& Stage: Stages)
		{
			Stage.Dump();
		}
		for(auto& ContestRankingEl: ContestRanking)
		{
			ContestRankingEl.Dump();
		}
		UE_LOG(LogDTFluxAPI, Log, TEXT("CONTEST DUMP END *****%s::%02d *****\n"), *Name, Id);
	}
	void SortContestRanking()
	{
		ContestRanking.Sort([](const FDTFluxContestRanking& A, const FDTFluxContestRanking& B)
		{
			if(A.Rank == 0 || B.Rank == 0)
				return true;
			return A.Rank > B.Rank;
		});
	};
};

USTRUCT(BlueprintType, Category="FDTFlux|Model")
struct FDTFluxFinisher
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="FDTFlux|Model")
	TEnumAsByte<EDTFluxFinisherType> Type;
	UPROPERTY(BlueprintReadOnly, Category="FDTFlux|Model")
	FDTFluxParticipant Participant;
	UPROPERTY(BlueprintReadOnly, Category="FDTFlux|Model")
	FDTFluxStageRanking CurrentRanking;
};


USTRUCT(BlueprintType, Category="DTFlux|Subsystem|Events")
struct FDTFluxWsResponseEvent
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	TEnumAsByte<EDTFluxResponseType> WsResponseType;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	FString RawData;
};
