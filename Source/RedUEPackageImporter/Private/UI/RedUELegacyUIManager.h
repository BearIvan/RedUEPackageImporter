#pragma once


class FRedUELegacyUIManager
{
public:
	void Initialize	();
	void Destroy	();
	
	TSharedPtr< FUICommandList>						UICommandList;
};
