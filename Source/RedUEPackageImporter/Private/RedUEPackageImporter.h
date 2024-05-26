#pragma once
#include "UI/RedUELegacyUIManager.h"


class FRedUEPackageImporterModule : public IModuleInterface
{
public:

	void StartupModule() override;
	void ShutdownModule() override;
private:
	FRedUELegacyUIManager UIManager;
};
