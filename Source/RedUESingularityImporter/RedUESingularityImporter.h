#pragma once

#include "Modules/ModuleManager.h"

class FRedUESingularityImporterModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};
