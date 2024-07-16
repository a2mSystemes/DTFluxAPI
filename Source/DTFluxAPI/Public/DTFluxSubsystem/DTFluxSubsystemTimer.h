// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxUtils/DTFluxEnums.h"
#include "Subsystems/WorldSubsystem.h"
#include "DTFluxSubsystemTimer.generated.h"

/**
 * 
 */

class UDTFluxDataStorage;
class UDTFluxSubsystem;

USTRUCT()
struct FDTFluxContestTimerHandle
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	int ContestId;	
	UPROPERTY()
	int StageId;
	UPROPERTY();
	TEnumAsByte<EDTFluxTimerEventType> Type;
	UPROPERTY();
	FTimerHandle Handle;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCutoff, int, ContestId, int, StageId);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStageStarted, int, ContestId, int, StageId);


UCLASS(BlueprintType, Category="DTFlux|Timer")
class DTFLUXAPI_API UDTFluxSubsystemTimer : public UTickableWorldSubsystem
{
	GENERATED_BODY()
public:
	/** Implement this for initialization of instances of the system */
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** Implement this for deinitialization of instances of the system */
	virtual void Deinitialize() override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(UDTFluxSubsystemTimer, STATGROUP_Tickables);
	}
	
	UPROPERTY()
	TArray<FDTFluxContestTimerHandle> Timers;
	
	
	UPROPERTY(BlueprintAssignable, Category="DTFlux|Timer")
	FOnCutoff OnCutoff;

	UPROPERTY(BlueprintAssignable, Category="DTFlux|Timer")
	FOnStageStarted OnStageStarted;

	UFUNCTION()
	void OnDataStorageInit();

	void AddCutoffTimer(const int ContestID, const int StageID);
	void AddStageStartedTimer(const int ContestID, const int StageID);

	void OnStartTimer();
	void OnCutOffTimer();

	void ClearTimer(FDTFluxContestTimerHandle TimerHandle);
	void ClearTimer(const int HandleIndex);

	static UDTFluxSubsystem* GetDTFluxSubSystem();
	static UDTFluxDataStorage* GetDTFluxDataStorage();

	static float GetSecondsFrom(const FDateTime When);
};


