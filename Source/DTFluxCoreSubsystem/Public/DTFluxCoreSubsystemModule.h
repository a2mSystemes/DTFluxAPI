#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DTFLUXCORESUBSYSTEM_API DECLARE_LOG_CATEGORY_EXTERN(logDTFluxCoreSubsystem, All, All);

class DTFLUXCORESUBSYSTEM_API FDTFluxCoreSubsystemModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
