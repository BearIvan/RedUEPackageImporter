#pragma once
#include "KismetNodes/SGraphNodeK2Event.h"

class UK2Node_SequenceAction;

class SGraphNodeK2SequenceAction:public SGraphNodeK2Event
{
public:
	SLATE_BEGIN_ARGS(SGraphNodeK2SequenceAction){}
	SLATE_END_ARGS()

			void		Construct					(const FArguments& InArgs, class UK2Node_SequenceAction* InNode);
protected:
	virtual void		CreateInputSideAddButton	(TSharedPtr<SVerticalBox> OutputBox) override;
	virtual EVisibility IsAddPinButtonVisible		() const override;
	virtual FReply		OnAddPin					() override;
	
	UK2Node_SequenceAction* SequenceNode = nullptr;
};
