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
