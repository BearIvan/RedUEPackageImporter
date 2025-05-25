#include "LegacyKismetBlueprintEditor.h"

#include "LegacyKismetNodeBindingExtension.h"

void FLegacyKismetBlueprintEditor::InitKismetBlueprintEditor(const EToolkitMode::Type Mode, const TSharedPtr<class IToolkitHost>& InitToolkitHost, const TArray<class UBlueprint*>& InBlueprints, bool bShouldOpenInDefaultsMode)
{
	InitBlueprintEditor(Mode, InitToolkitHost, InBlueprints, bShouldOpenInDefaultsMode);
	
	Inspector->GetPropertyView()->SetExtensionHandler(MakeShared<FLegacyKismetNodeBindingExtension>());
}
