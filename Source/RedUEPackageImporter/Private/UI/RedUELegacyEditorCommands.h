#pragma once

class SRedUEPackageExplorer: public  SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRedUEPackageExplorer)
	{
		
	}
	
	SLATE_END_ARGS()

	void	Construct	(const FArguments& InArgs);
protected:
	FReply	TestClicked	();
    FReply TestClicked2();
};
