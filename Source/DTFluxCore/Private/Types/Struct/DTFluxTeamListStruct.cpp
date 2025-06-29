// Fill out your copyright notice in the Description page of Project Settings.


#include "Types/Struct/DTFluxTeamListStruct.h"


bool FDTFluxParticipant::IsTeam() const
{
	return Person2.FirstName.IsEmpty() && Person2.LastName.IsEmpty();
}


void FDTFluxParticipant::Dump() const
{
	FString EliteStr = "NO";
	if(Elite)
	{
		EliteStr = "YES";
	}
	UE_LOG(logDTFluxCore, Log, TEXT("PARTICIPANT with bib: %03d"), Bib);
	UE_LOG(logDTFluxCore, Log, TEXT("Fullname : %s %s"), *Person1.FirstName, *Person1.LastName);
	if(IsTeam())
	{
		UE_LOG(logDTFluxCore, Log, TEXT("Teamate : %s %s"), *Person2.FirstName, *Person2.LastName);
		UE_LOG(logDTFluxCore, Log, TEXT("Team name : %s"), *Team);
	}
	UE_LOG(logDTFluxCore, Log, TEXT("Club : %s, Category : %s, IsElite : %s, Status : %s"),
		*Club, *Category, *EliteStr, *UEnum::GetValueAsString(Status));
}

FString FDTFluxParticipant::GetParticipantFormatedName(bool Truncate, int MaxSize) const
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
				UE_LOG(logDTFluxCore, Log, TEXT("Firstname is with space compound. Formated Name %s length %02d MAX Size : %02d"),
					*Formated, Formated.Len(), MaxSize);
				if(Formated.Len() >= MaxSize)
				{
					UE_LOG(logDTFluxCore, Log, TEXT("Reducing %s Formated"), *Formated);

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
				UE_LOG(logDTFluxCore, Log, TEXT("Firstname is with space compound. Formated Name %s length %02d MAX Size : %02d"),
					*Formated, Formated.Len(), MaxSize);
				if(Formated.Len() >= MaxSize)
				{
					UE_LOG(logDTFluxCore, Log, TEXT("Reducing %s Formated"), *Formated);
					return Formated.Left(MaxSize - 3).Append("...");
				}
				return Formated;
			}
			FString Formated = Person1.FirstName.Left(1).Append(". ");
			Formated.Append(Person1.LastName);
			UE_LOG(logDTFluxCore, Log, TEXT("Firstname is not compound. Formated Name %s length %02d MAX Size : %02d"),
				*Formated, Formated.Len(), MaxSize);
			if(Formated.Len() >= MaxSize)
			{
				UE_LOG(logDTFluxCore, Log, TEXT("Reducing %s Formated"), *Formated);
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
