#include "LegacyXFloatingSection.h"
#include "Core/RedUELegacySubsystem.h"
#include "Entities/XFloatingSection.h"
#include "World/LegacyWorld.h"

void ULegacyXFloatingSection::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	AXFloatingSection* FloatingSection = CastChecked<AXFloatingSection>(InActor);
	URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	if (RedUELegacySubsystem->FloatingSectionIndexTable)
	{
		for (const FLegacyXWorldFloatingSectionIndexData& IndexData :RedUELegacySubsystem->FloatingSectionIndexTable->MapData)
		{
			if (IndexData.ActorGuid == FloatingSectionGuid)
			{
				FloatingSection->LevelNames = IndexData.LevelNames;
			}
		}
	}
	FloatingSection->InvInitialTransform = InvInitialTransform;
	
}

UClass* ULegacyXFloatingSection::GetActorClass_Implementation()
{
	return AXFloatingSection::StaticClass();
}
