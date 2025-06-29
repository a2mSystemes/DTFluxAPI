// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxGeneralSettings.h"

#include "DTFluxProjectSettingsModule.h"

UDTFluxGeneralSettings::UDTFluxGeneralSettings()
{
	CategoryName = "DTFlux API";

	UE_LOG(logDTFluxProjectSettings, Log, TEXT("ModelAssetLoded isNull() -> %s"), ModelAsset.IsNull() ? TEXT("TRUE") : TEXT("FALSE"));

	UE_LOG(logDTFluxProjectSettings, Log, TEXT("ModelAssetLoded IsValid() -> %s"), ModelAsset.IsValid() ? TEXT("TRUE") : TEXT("FALSE"));
}
