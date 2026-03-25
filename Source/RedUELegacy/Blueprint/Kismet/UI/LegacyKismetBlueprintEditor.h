#pragma once
#include "BlueprintEditor.h"

class FLegacyKismetBlueprintEditor:public FBlueprintEditor
{
public:
			void 						InitKismetBlueprintEditor	(const EToolkitMode::Type Mode, const TSharedPtr< class IToolkitHost >& InitToolkitHost, const TArray<class UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode);
						
private:						
			void 						OnRemoveEventPin			();
			bool 						CanAddRemovePin				();
	virtual void 						OnCreateGraphEditorCommands	(TSharedPtr<FUICommandList> GraphEditorCommandsList) override;

protected:
	virtual TSubclassOf<UEdGraphSchema> GetDefaultSchemaClass		() const override;
};
