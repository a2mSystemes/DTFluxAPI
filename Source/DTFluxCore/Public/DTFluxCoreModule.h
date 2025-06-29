#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

/**
 * @module DTFluxCoreModule
 * @details DTFlux Project is a framework to integrate all kind of events data from
 * multiple API (stopwatch servers, etc...) or manually to unreal motion design platform
 * to create live audiovisual shows. 
 * @brief This module provides all necessary basic class to set up your communication
 * between an external APIs and unreal engine.
 * @license See LICENSE.TXT at the of DTFluxAPI plugin folder or at
 * @see https://github.com/A2MSystemes/DTFluxAPI/blob/main/LICENSE
 * @author Ange-Marie MAURIN
 */

DTFLUXCORE_API DECLARE_LOG_CATEGORY_EXTERN(logDTFluxCore, Log, All);

class DTFLUXCORE_API FDTFluxCoreModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
