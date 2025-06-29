#include "DTFluxCoreModule.h"

#define LOCTEXT_NAMESPACE "FDTFluxCoreModule"

DTFLUXCORE_API DEFINE_LOG_CATEGORY(logDTFluxCore);

void FDTFluxCoreModule::StartupModule()
{
    
}

void FDTFluxCoreModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDTFluxCoreModule, DTFluxCore)