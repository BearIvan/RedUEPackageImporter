#include "Sounds/LegacyXSound.h"
#include "Editor.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacySubsystem.h"
#include "Sounds/WWiseBanksManager.h"

UObject* ULegacyXAKAudioEventID::ExportToContent()
{
	if (!PresentObject)
	{
		if (URedUELegacySubsystem* RedUELegacySubsystem = GetTypedOuter<URedUELegacySubsystem>())
		{
			PresentObject = RedUELegacySubsystem->WWiseBanksManager->ExportToContent(FPaths::GetBaseFilename( LegacyPackage->FileName),Id);
		}
	}
	return PresentObject;
}
