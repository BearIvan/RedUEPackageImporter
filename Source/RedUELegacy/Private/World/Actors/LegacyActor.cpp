#include "World/Actors/LegacyActor.h"

#include "Core/RedUELegacyArchive.h"

AActor* ULegacyActor::Spawn_Implementation()
{
	ensure(PresentObject == nullptr);
	AActor* Actor = GWorld->SpawnActor<AActor>(GetActorClass(),FVector(Location),FRotator(Rotation));
	Actor->SetActorScale3D(FVector(DrawScale3D)*DrawScale);
	FillActor(Actor);
	PresentObject = Actor;
	return Actor;
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

void ULegacyActor::FillActor_Implementation(AActor* InActor)
{
}

UClass* ULegacyActor::GetActorClass_Implementation()
{
	return AActor::StaticClass();
}
