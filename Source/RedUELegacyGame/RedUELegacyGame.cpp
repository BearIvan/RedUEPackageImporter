#include "RedUELegacyGame.h"

#include "Interfaces/IPluginManager.h"

#define LOCTEXT_NAMESPACE "FRedUELegacyGameModule"

void FRedUELegacyGameModule::StartupModule()
{
	{
		FString PluginBaseDirectory = IPluginManager::Get().FindPlugin(TEXT("RedUEPackageImporter"))->GetBaseDir();
		// Append the local shader directory.
		FString ShaderDirectory = FPaths::Combine(PluginBaseDirectory, TEXT("Shaders"));
		// Construct the virtual path shorthand.
		FString VirtualShaderDirectory = FString::Printf(TEXT("/Plugin/%s"), TEXT("RedUEPackageImporter"));
		// Create a mapping to the virtual shader directory shorthand.
		AddShaderSourceDirectoryMapping(VirtualShaderDirectory, ShaderDirectory);
	}
}

void FRedUELegacyGameModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FRedUELegacyGameModule, RedUELegacyGame)