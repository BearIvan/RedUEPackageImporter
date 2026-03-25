#include "World/Actors/LegacyActor.h"

#include "Editor.h"
#include "Actors/XFloatingSection.h"
#include "Actors/XMatineeCameraActor.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"
#include "World/LegacyWorld.h"
#include "World/Components/LegacySkeletalMeshComponent.h"

AActor* ULegacyActor::Spawn_Implementation()
{
	ensure(PresentObject == nullptr);
	if (UClass* ActorClass = GetActorClass())
	{
		AActor* Actor = GWorld->SpawnActor<AActor>(ActorClass,FVector(Location),FRotator(Rotation));
		Actor->SetActorScale3D(FVector(DrawScale3D)*DrawScale);
		FillActor(Actor);
		PresentObject = Actor;
		return Actor;
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

UClass* ULegacyCameraActor::GetActorClass_Implementation()
{
	return ACameraActor::StaticClass();
}

UClass* ULegacyMarker::GetActorClass_Implementation()
{
	return ATargetPoint::StaticClass();
}

UClass* ULegacyXMatineeCameraActor::GetActorClass_Implementation()
{
	return AXMatineeCameraActor::StaticClass();
}


void ULegacyXMatineeCameraActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);

	AXMatineeCameraActor* CameraActor = CastChecked<AXMatineeCameraActor>(InActor);
	SkeletalMeshComponent->FillComponent(CameraActor->SkeletalMesh);
}

void ULegacyWorldInfo::GetStreamingLevels(TArray<ULegacyLevelStreaming*>& OutStreamingLevels)
{
	OutStreamingLevels = StreamingLevels;
}

UClass* ULegacyWorldInfo::GetActorClass_Implementation()
{
	return nullptr;
}

UClass* ULegacyPlayerStart::GetActorClass_Implementation()
{
	return APlayerStart::StaticClass();
}


void ULegacyXWorldInfo::GetStreamingLevels(TArray<ULegacyLevelStreaming*>& OutStreamingLevels)
{
	OutStreamingLevels = StreamingLevelsWrapper.StreamingLevels;
}

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

UClass* ULegacyEmitter::GetActorClass_Implementation()
{
	return AEmitter::StaticClass();
}

void ULegacyEmitter::FillActor_Implementation(AActor* InActor)
{
	if (AEmitter* InEmitter = CastChecked<AEmitter>(InActor))
	{
		if (ParticleSystemComponent)
		{
			ParticleSystemComponent->FillComponent(InEmitter->GetParticleSystemComponent());
		}
	}
}

void ULegacyActor::FillActor_Implementation(AActor* InActor)
{
}

UClass* ULegacyActor::GetActorClass_Implementation()
{
	return AActor::StaticClass();
}
