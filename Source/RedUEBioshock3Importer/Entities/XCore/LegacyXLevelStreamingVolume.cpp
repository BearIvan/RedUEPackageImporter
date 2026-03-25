#include "LegacyXLevelStreamingVolume.h"

#include "LegacyXWorldInfo.h"
#include "Core/RedUELegacySubsystem.h"
#include "Entities/XLevelStreamingVolume.h"
#include "World/LegacyWorld.h"


UClass* ULegacyXLevelStreamingVolume::GetActorClass_Implementation()
{
	return AXLevelStreamingVolume::StaticClass();
}

void ULegacyXLevelStreamingVolume::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);
	
	ULegacyXWorldInfo* WorldInfo = nullptr;
	URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	if (RedUELegacySubsystem->GLegacyWorld && RedUELegacySubsystem->GLegacyWorld->PersistentLevel)
	{
		WorldInfo = Cast<ULegacyXWorldInfo>(RedUELegacySubsystem->GLegacyWorld->PersistentLevel->WorldInfo);
	}
	if (AXLevelStreamingVolume* LevelStreamingVolume = Cast<AXLevelStreamingVolume>(InActor))
	{
		LevelStreamingVolume->bLevelStreamingActive = bLevelStreamingActive;
		for (const FXLevelAndStreamingStatus& InLevel :Levels)
		{
			if (WorldInfo)
			{
				bool bFound = false;
				for (UObject* InGroupedStreamingLevel : WorldInfo->StreamingLevelsWrapper.GroupedStreamingLevels)
				{
					if (UXLevelStreamingGroup* GroupedStreamingLevel = Cast<UXLevelStreamingGroup>(InGroupedStreamingLevel))
					{
						if (GroupedStreamingLevel&&GroupedStreamingLevel->GroupName == InLevel.LevelName)
						{
							for (ULegacyLevelStreaming* InStreamingLevel: GroupedStreamingLevel->StreamingLevels)
							{
								FXLevelAndStreamingStatus&OutStatus = LevelStreamingVolume->Levels.AddDefaulted_GetRef();
								OutStatus.DesiredStatus = InLevel.DesiredStatus;
								OutStatus.LevelName = InStreamingLevel->PackageName;
							}
							bFound  = true;
						}
					}
				}
				if (bFound)
				{
					continue;
				}
			}
			LevelStreamingVolume->Levels.Add(InLevel);
		}
	}
}
