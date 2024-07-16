// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AvaText3DComponent.h"
#include "Components/ActorComponent.h"
#include "DTFluxDataStorage/DTFluxDataStorage.h"
#include "DTFluxCountDownComponent.generated.h"





UCLASS(ClassGroup=(DTFlux), meta=(BlueprintSpawnableComponent))
class DTFLUXAPI_API UDTFluxCountDownComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDTFluxCountDownComponent();
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Counter")
	FDateTime GoTime;
	UPROPERTY(BlueprintSetter=SetDuration, Category="DTFlux|Counter")
	int32 Duration;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	UPROPERTY()
	UAvaText3DComponent* TextRef;
	FTimerHandle WaitingTimer;
	FTimerHandle ContDownTimer;
	int64 InternalDuration;
	bool IsWaiting;
	bool IsCounting;
	UDTFluxDataStorage* DataStorage;

	

public:
	
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Counter")
	FString EndString;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Counter")
	float WaitingRate = 1.0f;
	UPROPERTY(BlueprintReadWrite, Category="DTFlux|Counter")
	float CountDownRate = 1.0f;
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Counter")
	void SetGoTime(FDateTime NewGoTime);
	UFUNCTION(BlueprintCallable, Category="DTFlux|Counter")
	void SetDuration(int32 NewDuration);

	// set the current stage
	UFUNCTION(BlueprintCallable, Category="DTFlux|Counter")
	void SetStage(const int ContestId, const int StageId){};
	
	// set the current contest
	UFUNCTION(BlueprintCallable, Category="DTFlux|Counter")
	void SetContest(const int ContestId){};
	
	UFUNCTION(BlueprintCallable, Category="DTFlux|Counter")
	void SetTarget(UAvaText3DComponent* TextComponent);

	UFUNCTION()
	void CountUpTimerFn();

	UFUNCTION()
	void WaitingTimerFn();

};
