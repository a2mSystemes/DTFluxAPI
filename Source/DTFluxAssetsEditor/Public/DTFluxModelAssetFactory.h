// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DTFluxModelAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class DTFLUXASSETSEDITOR_API UDTFluxModelAssetFactory : public UFactory
{
	GENERATED_BODY()
public:
	UDTFluxModelAssetFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
