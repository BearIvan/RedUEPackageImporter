#pragma once


class FRedUELegacyUIManager
{
public:
	void Initialize		();
	void Destroy		();
	TSharedRef<SDockTab> CreateShowExplorerTab	(const FSpawnTabArgs& Args);
	TSharedPtr< FUICommandList>						UICommandList;
};
