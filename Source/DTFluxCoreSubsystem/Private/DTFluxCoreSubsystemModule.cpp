#include "DTFluxCoreSubsystemModule.h"

DTFLUXCORESUBSYSTEM_API DEFINE_LOG_CATEGORY(logDTFluxCoreSubsystem)

#define LOCTEXT_NAMESPACE "FDTFluxCoreSubsystemModule"

void FDTFluxCoreSubsystemModule::StartupModule()
{
}

void FDTFluxCoreSubsystemModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDTFluxCoreSubsystemModule, DTFluxCoreSubsystem)