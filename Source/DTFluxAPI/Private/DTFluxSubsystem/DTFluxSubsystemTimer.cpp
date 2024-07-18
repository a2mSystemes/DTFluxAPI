// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxSubsystem/DTFluxSubsystemTimer.h"

#include "DTFluxAPILog.h"
#include "DTFluxSubsystem/DTFluxSubsystem.h"

void UDTFluxSubsystemTimer::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	// Not Necessary Anymore
	// UDTFluxSubsystem* Subsystem = GetDTFluxSubSystem();
	// Subsystem->OnRaceDataReceived.AddDynamic(this, &UDTFluxSubsystemTimer::OnDataStorageInit);

}

void UDTFluxSubsystemTimer::Deinitialize()
{
	Super::Deinitialize();
}

void UDTFluxSubsystemTimer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}



void UDTFluxSubsystemTimer::OnDataStorageInit()
{
	UE_LOG(LogDTFluxAPI, Log, TEXT("DataStorage Has been Set Or Updated"));
	UDTFluxDataStorage* DataStorage = GetDTFluxDataStorage();
	for(const auto&Contest : DataStorage->Contests)
	{
		for (const auto& Stage: Contest.Stages)
		{

			UWorld* World = GetWorld();
			if(World)
			{
				FDTFluxContestTimerHandle StartContestTimerHandle;
				StartContestTimerHandle.Type = EDTFluxTimerEventType::StageStart;
				StartContestTimerHandle.ContestId = Contest.Id;
				StartContestTimerHandle.StageId = Stage.Id;
				
				FDTFluxContestTimerHandle CutOffContestTimerHandle;
				CutOffContestTimerHandle.Type = EDTFluxTimerEventType::StageCutOff;
				CutOffContestTimerHandle.ContestId = Contest.Id;
				CutOffContestTimerHandle.StageId = Stage.Id;
				
				float StartTimeTriggerSeconds = GetSecondsFrom(Stage.StartTime);
				float CutOffTimeTriggerSeconds = GetSecondsFrom(Stage.CutOff);
				if( StartTimeTriggerSeconds > 0)
				{
					UE_LOG(LogDTFluxAPI, Log, TEXT("Can Set Time to %04f Seconds"), StartTimeTriggerSeconds );
					World->GetTimerManager().SetTimer(StartContestTimerHandle.Handle, this, &UDTFluxSubsystemTimer::OnStartTimer, StartTimeTriggerSeconds);
					World->GetTimerManager().SetTimer(CutOffContestTimerHandle.Handle, this, &UDTFluxSubsystemTimer::OnCutOffTimer, CutOffTimeTriggerSeconds);
					Timers.Add(StartContestTimerHandle);
					Timers.Add(CutOffContestTimerHandle);
					
				}
				else
				{
					UE_LOG(LogDTFluxAPI, Warning, TEXT("Unable to Set Time to %04f Seconds"), StartTimeTriggerSeconds);
				}
			}
			
		}
	}
}

void UDTFluxSubsystemTimer::AddCutoffTimer(const int ContestID, const int StageID)
{
	UWorld* World = GetWorld();
	if(World)
	{
		FTimerHandle Timer;
		World->GetTimerManager().SetTimer(Timer, this, &UDTFluxSubsystemTimer::OnStartTimer, 1.0, true);
		UE_LOG(LogDTFluxAPI, Warning, TEXT("AddCutoffTimer Added"));
	}
	UE_LOG(LogDTFluxAPI, Error,
		TEXT("UDTFluxSubsystemTimer::AddCutoffTimer Cannot have the World"));
}


void UDTFluxSubsystemTimer::AddStageStartedTimer(const int ContestID, const int StageID)
{
	UWorld* World = GetWorld();
	if(World)
	{
		FTimerHandle Timer;
		World->GetTimerManager().SetTimer(Timer, this, &UDTFluxSubsystemTimer::OnStartTimer, 1.0, true);
		UE_LOG(LogDTFluxAPI, Warning, TEXT("AddStageStartedTimer Added"));
	}
	UE_LOG(LogDTFluxAPI, Error,
		TEXT("UDTFluxSubsystemTimer::AddStageStartedTimer Cannot have the World"));

}

void UDTFluxSubsystemTimer::OnStartTimer()
{
	UWorld* World = GetWorld();
	if(World)
	{
		int Idx = 0 ;
		for(auto& Timer : Timers)
		{
			if(Timer.Type == EDTFluxTimerEventType::StageStart)
			{
				if(World->GetTimerManager().GetTimerRemaining(Timer.Handle) == 0)
				{
					TArray<int> ContestIds;
					ContestIds.Add(Timer.ContestId);
					OnStageStarted.Broadcast(ContestIds, Timer.StageId);
					break;
				}
			}
			Idx++;
		}
		if(Timers.IsValidIndex(Idx))
		{
			Timers.RemoveAt(Idx);
		}
	}
}
void UDTFluxSubsystemTimer::OnCutOffTimer()
{
	UWorld* World = GetWorld();
	if(World)
	{
		int Idx = 0 ;
		for(auto& Timer : Timers)
		{
			if(Timer.Type == EDTFluxTimerEventType::StageCutOff)
			{
				if(World->GetTimerManager().GetTimerRemaining(Timer.Handle) == 0)
				{
					TArray<int> ContestIds;
					ContestIds.Add(Timer.ContestId);
					OnCutoff.Broadcast(ContestIds, Timer.StageId);
					break;
				}
			}
			Idx++;
		}
		if(Timers.IsValidIndex(Idx))
		{
			Timers.RemoveAt(Idx);
		}
	}
}

void UDTFluxSubsystemTimer::ClearTimer(FDTFluxContestTimerHandle TimerHandle)
{
	UWorld* World = GetWorld();
	if(World)
	{
		World->GetTimerManager().ClearTimer(TimerHandle.Handle);
	}
	UE_LOG(LogDTFluxAPI, Error, TEXT("Cannot Clear Timer %s of type %s for Stage%02d of Contest%02d"),
		*TimerHandle.Handle.ToString(), *UEnum::GetValueAsString(TimerHandle.Type),
		TimerHandle.StageId, TimerHandle.ContestId)
	
}

void UDTFluxSubsystemTimer::ClearTimer(const int HandleIndex)
{
}


void UDTFluxSubsystemTimer::TriggerOnCutOff(const TArray<int> ContestIds, const int StageId)
{
		OnCutoff.Broadcast(ContestIds, StageId);
}

void UDTFluxSubsystemTimer::TriggerStartTime(const TArray<int> ContestIds, const int StageId)
{
		OnStageStarted.Broadcast(ContestIds, StageId);
}

void UDTFluxSubsystemTimer::TriggerStageLoading(const TArray<int> ContestIds, const int StageId, int DelayBeforeStageStart)
{
		OnStageLoading.Broadcast(ContestIds, StageId, DelayBeforeStageStart);
}

void UDTFluxSubsystemTimer::TriggerOnDeleteRequested(const TArray<int> LineIndex)
{
	OnRemoveLineRequested.Broadcast(LineIndex);
}

UDTFluxSubsystem* UDTFluxSubsystemTimer::GetDTFluxSubSystem()
{
	return GEngine->GetEngineSubsystem<UDTFluxSubsystem>();
}

UDTFluxDataStorage* UDTFluxSubsystemTimer::GetDTFluxDataStorage()
{
	return GetDTFluxSubSystem()->GetDataStorage();
}

float UDTFluxSubsystemTimer::GetSecondsFrom(const FDateTime When)
{
	FTimespan Delta = When - FDateTime::Now();
	return static_cast<float>(Delta.GetTotalSeconds()) ;
}

