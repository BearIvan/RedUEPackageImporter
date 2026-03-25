#pragma once

#include "EdGraphSchema_K2.h"
#include "EdGraphSchema_LegacyKismet.generated.h"

UCLASS()
class REDUELEGACY_API UEdGraphSchema_LegacyKismet : public UEdGraphSchema_K2
{
	GENERATED_BODY()

public:
	virtual void GetContextMenuActions(UToolMenu* Menu, UGraphNodeContextMenuContext* Context) const override;
};
