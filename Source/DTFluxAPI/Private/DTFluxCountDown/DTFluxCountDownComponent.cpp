// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxCountDown/DTFluxCountDownComponent.h"

#include "DTFluxAPILog.h"
#include "DTFluxSubsystem/DTFluxSubsystem.h"


// Sets default values for this component's properties
UDTFluxCountDownComponent::UDTFluxCountDownComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	if(FModuleManager::Get().IsModuleLoaded("DTFluxApi"))
	{
		DataStorage = GEngine->GetEngineSubsystem<UDTFluxSubsystem>()->GetDataStorage();
		UE_LOG(LogDTFluxAPI, Log, TEXT("DTFluxApi loaded"))
	}else
	{
		UE_LOG(LogDTFluxAPI, Error, TEXT("DTFluxApi Not Loaded"))
	}
	// ...
}


// Called when the game starts
void UDTFluxCountDownComponent::BeginPlay()
{
	Super::BeginPlay();

	
	UWorld* World = GetWorld();
	if(World)
	{
		World->GetTimerManager().
		SetTimer(WaitingTimer, this,
			&UDTFluxCountDownComponent::WaitingTimerFn, WaitingRate, true);
	}
	// ...
	
}




// Called every frame
void UDTFluxCountDownComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                              FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDTFluxCountDownComponent::SetGoTime(FDateTime NewGoTime)
{
	GoTime = NewGoTime;
}

void UDTFluxCountDownComponent::SetDuration(int32 NewDuration)
{
	// Need to be protected while counting
	Duration = NewDuration;
}

void UDTFluxCountDownComponent::SetTarget(UAvaText3DComponent* TextComponent)
{
	// Need to be protected while counting
	TextRef = TextComponent;
	if(IsValid(TextRef))
	{
		FText WaitingText = FText::FromString("WAITING !!!");
		
		TextRef->SetText(WaitingText);

		UE_LOG(LogDTFluxAPI, Log, TEXT("Setting TextRef to %s"), *TextRef->GetText().ToString());
	}
}

void UDTFluxCountDownComponent::CountUpTimerFn()
{
	
}

void UDTFluxCountDownComponent::WaitingTimerFn()
{

	// UE_LOG(LogDTFluxAPI, Log, TEXT("WAITING"));
}

