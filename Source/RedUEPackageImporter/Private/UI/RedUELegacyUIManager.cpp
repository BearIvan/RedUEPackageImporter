#include "RedUELegacyUIManager.h"

void FRedUELegacyUIManager::Initialize()
{
	UICommandList = MakeShareable(new FUICommandList);
}

void FRedUELegacyUIManager::Destroy()
{
	UICommandList.Reset();
}
