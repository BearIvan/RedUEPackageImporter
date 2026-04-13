#include "LegacyActor.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"

AActor* ULegacyActor::Spawn_Implementation()
{
	ensure(PresentObject == nullptr);
	if (UClass* ActorClass = GetActorClass())
	{
		if (AActor* Actor = GWorld->SpawnActor<AActor>(ActorClass,FVector(Location),FRotator(Rotation)))
		{
			Actor->SetActorScale3D(FVector(DrawScale3D)*DrawScale);
			Actor->SetActorLabel(GetLegacyName());
			Actor->SetFolderPath(Group);
			if (!Tag.IsNone())
			{
				Actor->Tags.Add(Tag);
			}
			FillActor(Actor);
			PresentObject = Actor;
			return Actor;
		}
	}
	return nullptr;
}

void ULegacyActor::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		Rotation = XPrivateLocalRotation;
		Location = XPrivateLocalLocation;
	}
}

UObject* ULegacyActor::ExportToContent()
{
	return Super::ExportToContent();
}

void ULegacyActor::FillActor_Implementation(AActor* InActor)
{
}

UClass* ULegacyActor::GetActorClass_Implementation()
{
	return nullptr;
}
