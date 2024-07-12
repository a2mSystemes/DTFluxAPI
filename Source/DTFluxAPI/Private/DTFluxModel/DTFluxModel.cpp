// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxModel/DTFluxModel.h"

bool FDTFluxParticipant::IsTeam() const
{
	return (Person2.FirstName != "");
}

void FDTFluxParticipant::Dump() const{
	FString EliteStr = "NO";
	if(Elite)
	{
		EliteStr = "YES";
	}
	UE_LOG(LogDTFluxAPI, Log, TEXT("PARTICIPANT with bib: %03d"), Bib);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Fullname : %s %s"), *Person1.FirstName, *Person1.LastName);
	if(IsTeam())
	{
		UE_LOG(LogDTFluxAPI, Log, TEXT("Teamate : %s %s"), *Person2.FirstName, *Person2.LastName);
		UE_LOG(LogDTFluxAPI, Log, TEXT("Team name : %s"), *Team);
	}
	UE_LOG(LogDTFluxAPI, Log, TEXT("Club : %s, Category : %s, IsElite : %s, Status : %s"),
		*Club, *Category, *EliteStr, *UEnum::GetValueAsString(Status));

}

bool FDTFluxStage::SetStartTime(const FDateTime& ContestDate, const FString& TimeString)
{
	TArray<FString> TimeTokensStart;
	TimeString.ParseIntoArray(TimeTokensStart, TEXT(":"));
	const int32 HoursStart = FCString::Atoi(*TimeTokensStart[0]);
	const int32 MinutesStart = FCString::Atoi(*TimeTokensStart[1]);
	const int32 SecondsStart = FCString::Atoi(*TimeTokensStart[2]);
	StartTime = FDateTime(ContestDate.GetYear(), ContestDate.GetMonth(), ContestDate.GetDay(),
		HoursStart, MinutesStart, SecondsStart);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Setting StartTime For %s to %s"), *Name, *StartTime.ToString());
	return true;
}

bool FDTFluxStage::SetEndTime(const FDateTime& ContestDate, const FString& TimeString)
{
	TArray<FString> TimeTokens;
	TimeString.ParseIntoArray(TimeTokens, TEXT(":"));
	const int32 Hours = FCString::Atoi(*TimeTokens[0]);
	const int32 Minutes = FCString::Atoi(*TimeTokens[1]);
	const int32 Seconds = FCString::Atoi(*TimeTokens[2]);
	EndTime = FDateTime(ContestDate.GetYear(), ContestDate.GetMonth(), ContestDate.GetDay(),
		Hours, Minutes, Seconds);
	UE_LOG(LogDTFluxAPI, Log, TEXT("Setting EndTime For %s to %s"), *Name, *StartTime.ToString());
	return true;
}

bool FDTFluxContest::SetDate(const FString& StringDate)
{
	TArray<FString> Tokens;
	StringDate.ParseIntoArray(Tokens, TEXT("-"));
	if(Tokens.Num() != 3)
	{
		return false;
	}
	const int32 Year = FCString::Atoi(*Tokens[0]);
	const int32 Month = FCString::Atoi(*Tokens[1]);
	const int32 Day = FCString::Atoi(*Tokens[2]);
	Date = FDateTime(Year, Month, Day);
	return true;
}
