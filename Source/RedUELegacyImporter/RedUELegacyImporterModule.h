#pragma once

class FRedUELegacyImporterModule : public IModuleInterface
{
public:

    void StartupModule() override;
    void ShutdownModule() override;
private:
    
    FDelegateHandle HybridMaterialTrackEditorHandle;
};
