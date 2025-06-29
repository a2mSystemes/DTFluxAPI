#include "DTFluxProjectSettingsModule.h"

#define LOCTEXT_NAMESPACE "FDTFluxProjectSettingsModule"

DTFLUXPROJECTSETTINGS_API DEFINE_LOG_CATEGORY(logDTFluxProjectSettings)

void FDTFluxProjectSettingsModule::StartupModule()
{
    
}

void FDTFluxProjectSettingsModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FDTFluxProjectSettingsModule, DTFluxProjectSettings)