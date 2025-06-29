// Fill out your copyright notice in the Description page of Project Settings.


#include "DTFluxAssetModelTypeActions.h"

#include "Assets/DTFluxModelAsset.h"




void FDTFluxAssetModelTypeActions::OpenAssetEditor(const TArray<UObject*>& InObjects,
	TSharedPtr<IToolkitHost> EditWithinLevelEditor)
{
	FAssetTypeActions_Base::OpenAssetEditor(InObjects, EditWithinLevelEditor);
}

UClass* FDTFluxAssetModelTypeActions::GetSupportedClass() const
{
	return UDTFluxModelAsset::StaticClass();
}

FText FDTFluxAssetModelTypeActions::GetName() const
{
	return INVTEXT("DTFluxModelAsset");
}

FColor FDTFluxAssetModelTypeActions::GetTypeColor() const
{
	return FColor(231, 81, 67);
}

uint32 FDTFluxAssetModelTypeActions::GetCategories()
{
	return Category;
}
