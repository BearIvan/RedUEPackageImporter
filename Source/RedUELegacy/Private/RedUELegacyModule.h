#pragma once

class FRedUELegacyModule : public IModuleInterface
{
public:

    void StartupModule() override;
    void ShutdownModule() override;
};
