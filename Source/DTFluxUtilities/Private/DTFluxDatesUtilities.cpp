// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxDatesUtilities.h"

#include "DTFluxUtilitiesModule.h"


DTFluxDatesUtilities::DTFluxDatesUtilities()
{
}

DTFluxDatesUtilities::~DTFluxDatesUtilities()
{
}

bool DTFluxDatesUtilities::CompileDateAndTime(const FString& Time, const FString& Date, FDateTime& OutDateTime)
{
	if(Time.Len() < 8 && Date.Len() < 10)
	{
		TArray<FString> ExplodedTime;
		Time.ParseIntoArray(ExplodedTime, TEXT(":"));
		if(ExplodedTime.Num() != 3 && !ExplodedTime[0].IsNumeric() && !ExplodedTime[1].IsNumeric() & !ExplodedTime[2].IsNumeric())
		{
			UE_LOG(logDTFluxUtilities, Error, TEXT("Bad Time Format [%s]. Unable to parse"), *Time);
			return false;
		}
		TArray<FString> ExplodedDate;
		Date.ParseIntoArray(ExplodedDate, TEXT("-"));
		if(ExplodedDate.Num() != 3 && !ExplodedDate[0].IsNumeric() && !ExplodedDate[1].IsNumeric() && !ExplodedDate[2].IsNumeric() )
		{
			UE_LOG(logDTFluxUtilities, Error, TEXT("Bad Date Format [%s]. Unable to parse"), *Date);
			return false;
		}
		int32 Hours = FCString::Atoi(*ExplodedTime[0]);
		int32 Minutes = FCString::Atoi(*ExplodedTime[1]);
		int32 Seconds = FCString::Atoi(*ExplodedTime[2]);
		int32 Day = FCString::Atoi(*ExplodedDate[2]);
		int32 Month = FCString::Atoi(*ExplodedDate[1]);
		int32 Year = FCString::Atoi(*ExplodedDate[0]);
		if(FDateTime::Validate(Year, Month, Day, Hours, Minutes, Seconds, 0))
		{
			OutDateTime = FDateTime(Year, Month, Day, Hours, Minutes, Seconds);
			return true;
		}
	}
	return false;
}

bool DTFluxDatesUtilities::CompileDateAndTime(const FDateTime& Time, const FDateTime& Date, FDateTime& OutDateTime)
{
	return false;
}
