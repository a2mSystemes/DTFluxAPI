#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DTFLUXUTILITIES_API DECLARE_LOG_CATEGORY_EXTERN(logDTFluxUtilities, All, All);

class DTFLUXUTILITIES_API FDTFluxUtilitiesModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
