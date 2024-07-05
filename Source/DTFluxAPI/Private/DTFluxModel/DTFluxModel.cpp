// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxModel/DTFluxModel.h"

void FDTFluxSplit::Dump() const
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("[DTFluxStage DUMP] Split ID : %i, Name:%s"), Id, *Name);

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

bool FDTFluxStage::UpdateStageRanking(TArray<TSharedPtr<FJsonValue>> Data)
{
	return true;
}

bool FDTFluxStage::AddSplit(TArray<TSharedPtr<FJsonValue>> SplitData)
{
	return true;
}

void FDTFluxStage::Dump() const
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("[DTFluxStage DUMP] Id : %i, Name : %s"), Id, *Name);
	UE_LOG(LogDTFluxAPI, Log, TEXT("[DTFluxStage DUMP] StartTime : %s, EndTime : %s"), *StartTime.ToString(), *EndTime.ToString());
	UE_LOG(LogDTFluxAPI, Log, TEXT("[DTFluxStage DUMP] Splits ["));
	for(const auto& Split : Splits)
	{
		Split.Dump();
	}
	UE_LOG(LogDTFluxAPI, Log, TEXT("[DTFluxStage DUMP] ]"));

		
}

bool FDTFluxContest::AddStage( const TArray<TSharedPtr<FJsonValue>> StagesData, TArray<FDTFluxSplit> Splits)
{
	for (const auto& StageData : StagesData)
	{
		FDTFluxStage Stage;
		Stage.Id = StageData->AsObject()->GetIntegerField(TEXT("id"));
		Stage.Name = StageData->AsObject()->GetStringField(TEXT("name"));
		FString StartTime = StageData->AsObject()->GetStringField(TEXT("startTime"));
		FString EndTime = StageData->AsObject()->GetStringField(TEXT("endTime"));
		Stage.SetStartTime(Date, StartTime);
		Stage.SetEndTime(Date, EndTime);
		Stage.Splits = Splits;
		Stages.Add(Stage);
		Stage.Dump();
	}
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
