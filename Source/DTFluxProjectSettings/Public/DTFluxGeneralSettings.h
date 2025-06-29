// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Assets/DTFluxModelAsset.h"
#include "Engine/DeveloperSettings.h"
#include "DTFluxGeneralSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Engine, DefaultConfig, meta=(DisplayName="DTFlux General Settings"))
class DTFLUXPROJECTSETTINGS_API UDTFluxGeneralSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:

	UDTFluxGeneralSettings();
	UPROPERTY(Category="General", Config, EditAnywhere, BlueprintReadOnly, DisplayName="Datastorage File")
	TSoftObjectPtr<UDTFluxModelAsset> ModelAsset;

};
