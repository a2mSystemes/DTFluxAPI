// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxUtils/DTFluxUtils.h"

#include "DTFluxModel/DTFluxModel.h"

EDTFluxStageStatusType UDTFluxModelHelper::GetStatusType(const int ContestID, const int StageID,
	UDTFluxDataStorage* DataStorage)
{
	EDTFluxStageStatusType StageStatus = UnknownStatus;

	FDTFluxStage SelectedStage;
	if( DataStorage->GetStage(ContestID, StageID, SelectedStage))
	{
		StageStatus = StageWaiting;
		FDateTime Now = FDateTime::Now();
		if(SelectedStage.StartTime <= Now)
		{
			StageStatus = StageStarted;
		}
		if(SelectedStage.CutOff <= Now)
		{
			StageStatus = StageEnded;
		}
	}
	return StageStatus;
}

int UDTFluxModelHelper::GetCurrentContest(UDTFluxDataStorage* DataStorage)
{
	int ContestId = -1;
	FDateTime Now = FDateTime::Now();
	for(const auto& Contest : DataStorage->Contests)
	{
		for(const auto& Stage : Contest.Stages)
		{
			// Stage has begun
			if(Stage.StartTime <= Now)
			{
				return Contest.Id;
			}
		}
	}
	return ContestId;
}

TArray<int> UDTFluxModelHelper::GetCurrentStage(UDTFluxDataStorage* DataStorage)
{
	TArray<int> ContestAndStageId;
	FDateTime Now = FDateTime::Now();
	for(const auto& Contest : DataStorage->Contests)
	{
		for(const auto& Stage : Contest.Stages)
		{
			// Stage has begun
			if(Stage.StartTime <= Now)
			{
				ContestAndStageId.Add(Contest.Id);
				ContestAndStageId.Add(Stage.Id);
				return ContestAndStageId;
			}
		}
	}
	return ContestAndStageId;
}

FString UDTFluxModelHelper::GetCurrentStageName(UDTFluxDataStorage* DataStorage)
{
	FString Name;
	FDateTime Now = FDateTime::Now();
	for(const auto& Contest : DataStorage->Contests)
	{
		for(const auto& Stage : Contest.Stages)
		{
			// Stage has begun
			if(Stage.StartTime <= Now)
			{
				Name = FString::Printf(TEXT("Contest %s Stage %s"), *Contest.Name, *Stage.Name);
				return Name;
			}
		}
	}
	return Name;
}
