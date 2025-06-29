#include "DTFluxAssetsEditorModule.h"
#include "IAssetTools.h"
#include "DTFluxAssetModelTypeActions.h"
#include "IAssetTools.h"
#include "AssetToolsModule.h"

#define LOCTEXT_NAMESPACE "FDTFluxAssetsEditorModule"


DTFLUXASSETSEDITOR_API DEFINE_LOG_CATEGORY(logDTFluxAssetEditor)
void FDTFluxAssetsEditorModule::StartupModule()
{
	IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
	EAssetTypeCategories::Type Category = AssetToolsModule.RegisterAdvancedAssetCategory("DTFlux", INVTEXT("DTFlux"));
    DTFluxAssetModelActions = MakeShareable(new FDTFluxAssetModelTypeActions(Category));
	AssetToolsModule.RegisterAssetTypeActions(DTFluxAssetModelActions.ToSharedRef());
}

void FDTFluxAssetsEditorModule::ShutdownModule()
{
	if(DTFluxAssetModelActions.IsValid() && FModuleManager::Get().IsModuleLoaded("AssetTools"))
	{
		IAssetTools& AssetToolsModule = FModuleManager::GetModuleChecked<FAssetToolsModule>("AssetTools").Get();
		AssetToolsModule.UnregisterAssetTypeActions(DTFluxAssetModelActions.ToSharedRef());
	}
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDTFluxAssetsEditorModule, DTFluxAssetsEditor)