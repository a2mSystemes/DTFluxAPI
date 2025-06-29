// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxModelAssetFactory.h"

#include "DTFluxAssetsEditorModule.h"
#include "Assets/DTFluxModelAsset.h"



UDTFluxModelAssetFactory::UDTFluxModelAssetFactory(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SupportedClass = UDTFluxModelAsset::StaticClass();
	bCreateNew = true;
}

// bool UDTFluxModelAssetFactory::CanCreateNew() const
// {
// 	return true;
// }

UObject* UDTFluxModelAssetFactory::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName,
                                                    EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UE_LOG(logDTFluxAssetEditor, Warning, TEXT("InParent Name %s"), InParent != nullptr ? *InParent->GetName() : TEXT("Parent is null !!!"))
	return NewObject<UDTFluxModelAsset>(InParent, InClass, InName, Flags, Context);
}
