#include "RedUEPackageImporter.h"
DEFINE_LOG_CATEGORY(RedUEPackageImporter);

void FRedUEPackageImporterModule::StartupModule()
{
	if(GIsEditor)
	{
		UIManager.Initialize();
	}
}

void FRedUEPackageImporterModule::ShutdownModule()
{
	if(GIsEditor)
	{
		UIManager.Destroy();
	}
}

IMPLEMENT_MODULE(FRedUEPackageImporterModule, RedUEPackageImporter)
