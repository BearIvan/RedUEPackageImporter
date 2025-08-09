#include "Sounds/LegacyXSound.h"
#include "Editor.h"
#include "WWiseBanksManager.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacySubsystem.h"

UObject* ULegacyXAKAudioEventID::ExportToContent()
{
	if (!PresentObject)
	{
		URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
		PresentObject = RedUELegacySubsystem->WWiseBanksManager->ExportToContent(FPaths::GetBaseFilename( LegacyPackage->FileName),Id);
	}
	return PresentObject;
}
