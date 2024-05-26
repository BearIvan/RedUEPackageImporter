#include "RedUEPackageImporter.h"


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
