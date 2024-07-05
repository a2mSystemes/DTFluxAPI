// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxProjectSettings/DTFluxProjectSettings.h"

const UDTFluxProjectSettings* UDTFluxProjectSettings::GetDTFluxAPIProjectSettings()
{
	return GetDefault<UDTFluxProjectSettings>();
}

void UDTFluxProjectSettings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FString ChangeKey = PropertyChangedEvent.Property->GetName();
	UE_LOG(LogDTFluxAPI, Log, TEXT("Settings %s has changed"), *ChangeKey);
	OnProjectSettingsModified.Broadcast(ChangeKey, GetDTFluxAPIProjectSettings());
}

UDTFluxProjectSettings::UDTFluxProjectSettings()
{
	CategoryName = "DTFlux Settings";
}
