#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"


/**
 * @module DTFluxAssetsEditorModule
 * @details DTFlux Project is a framework to integrate all kind of events data from
 * multiple API (stopwatch servers, etc...) or manually to unreal motion design platform
 * to create live audiovisual shows. 
 * @brief This module provides all editor and ui tools.
 * @license See LICENSE.TXT at the of DTFluxAPI plugin folder or at
 * @see https://github.com/A2MSystemes/DTFluxAPI/blob/main/LICENSE
 * @author Ange-Marie MAURIN
 */

class FDTFluxAssetModelTypeActions;
DTFLUXASSETSEDITOR_API DECLARE_LOG_CATEGORY_EXTERN(logDTFluxAssetEditor, Log, All);

class FDTFluxAssetsEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

private:
	TSharedPtr<FDTFluxAssetModelTypeActions> DTFluxAssetModelActions;
};
