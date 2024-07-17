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
	void Dump () const
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
	FDateTime TimeStart;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	float SpeedRunning;	
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	float SpeedTotal;	
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	float SpeedSwim;
	
	

	void Dump() const
	{	
		UE_LOG(LogDTFluxAPI, Log, TEXT("RANKING : %02d. Participant bib %d %s %s %s %s %s"),
			Rank, Bib, *Gap, *TimeSwim,
			*TimeTransition, *TimeRun, *TimeStart.ToString());
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
	int Rank = 0;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	bool Display = false;
	void Dump() const
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("SplitGapItem"))
		// Participant.Dump();
		UE_LOG(LogDTFluxAPI, Log, TEXT("Bib %02d Rank %02d Gap %s Time %s"), Bib, Rank, *Gap, *Time);
	}
};

USTRUCT(BlueprintType, Category="DTFlux|Model")
// ReSharper disable once IdentifierTypo
struct DTFLUXAPI_API FDTFluxFinisherData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int ContestId;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int StageId;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	int Bib = -1;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxSplitRanking SplitRanking;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Model")
	FDTFluxStageRanking StageRanking;
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
	TArray<FDTFluxSplitRanking> SplitRankings;

	void Dump() const
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Split %02d::%s *****\n"), Id, *Name);
		for(const auto& SplitGapItem : SplitRankings)
		{
			SplitGapItem.Dump();
		}
	}

	void InsertOrReplace(const FDTFluxStageRankingResponseItem& SplitRankingItemResp)
	{
		FDTFluxSplitRanking NewSplitGapItem;
		NewSplitGapItem.Bib = SplitRankingItemResp.Bib;
		NewSplitGapItem.Gap = SplitRankingItemResp.Gap;
		NewSplitGapItem.Rank = SplitRankingItemResp.Rank;
		NewSplitGapItem.Time = SplitRankingItemResp.Time;
		if(SplitRankings.IsEmpty())
		{
			SplitRankings.Add(NewSplitGapItem);
			return;
		}
		bool Update = true;
		int Idx = 0;
		for(auto& SplitGapItem : SplitRankings)
		{
			if(SplitGapItem.Bib == SplitRankingItemResp.Bib)
			{
				Update = false;
			}
			Idx++;
		}
		if(Update)
		{
			if(SplitRankings.IsValidIndex(Idx))
			{
				SplitRankings.RemoveAt(Idx);
			}
		}
		SplitRankings.Add(NewSplitGapItem);
	};

	void SortByRank()
	{
		SplitRankings.Sort([](const FDTFluxSplitRanking& A, const FDTFluxSplitRanking& B)
		{
			if(A.Rank == 0 && B.Rank == 0)
				return true;
			return A.Rank < B.Rank;
		});
	}

	TArray<FDTFluxSplitRanking> GetSplitRanking(const int From = 0, const int DisplayNumber = 0)
	{
		TArray<FDTFluxSplitRanking> NewSplitRankings;
		SortByRank();
		NewSplitRankings.Append(SplitRankings);

		if(From == 0 && DisplayNumber == 0)
			return NewSplitRankings;
		for(auto& SRank : SplitRankings)
		{
			if(SRank.Rank >= From)
			{
				NewSplitRankings.Add(SRank);
				if(NewSplitRankings.Num() >= DisplayNumber)
				{
					return NewSplitRankings;
				}
			}
		}
		return NewSplitRankings;
		
	}
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
	FDateTime CutOff;
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

	}

	EDTFluxSplitType GetSplitType(int SplitID)
	{
		int SplitCount = Splits.Num();
		//sort by ID
		Splits.Sort([](const FDTFluxSplit& A, const FDTFluxSplit& B)
		{
			return A.Id < B.Id;
		});
		int SplitIndex = Splits.IndexOfByPredicate([SplitID](const FDTFluxSplit& Split)
		{
			return Split.Id == SplitID;
		});
		
		if(SplitCount -2 == SplitIndex )
		{
			return EDTFluxSplitType::PreFinnishSplit;
		}
		if(SplitCount -1 == SplitIndex)
		{
			return EDTFluxSplitType::FinishSplit;
		}
		return EDTFluxSplitType::NormalSplit;
		
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

		ContestRanking.Add(NewRanking);
		return true;
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
struct DTFLUXAPI_API FDTFluxFinisher
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
struct DTFLUXAPI_API FDTFluxStageFinished
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	int ContestId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	int StageId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	TArray<FDTFluxStageRanking> Rankings;
};

USTRUCT(BlueprintType, Category="DTFlux|Subsystem|Events")
struct DTFLUXAPI_API FDTFluxContestFinished
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	int ContestId = 0;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem|Events")
	TArray<FDTFluxStageRanking> Rankings;
};

USTRUCT(BlueprintType, Category="DTFlux|Subsystem")
struct FDTFluxPoursuit
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem")
	int Bib;
	UPROPERTY(BlueprintReadOnly, Category="DTFlux|Subsystem")
	FDateTime TimeStart;
};