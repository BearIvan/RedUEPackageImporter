#include "World/Actors/LegacyActor.h"

#include "Editor.h"
#include "Actors/XFloatingSection.h"
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


void ULegacyXMatineeCameraActor::FillActor_Implementation(AActor* InActor)
{
	Super::FillActor_Implementation(InActor);

	ACameraActor* CameraActor = CastChecked<ACameraActor>(InActor);
	
	USkeletalMeshComponent* InSkeletalMeshComponent = NewObject< USkeletalMeshComponent>(CameraActor, NAME_None, RF_Transactional);
	CameraActor->AddInstanceComponent(InSkeletalMeshComponent);
	InSkeletalMeshComponent->AttachToComponent(CameraActor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	InSkeletalMeshComponent->OnComponentCreated();
	InSkeletalMeshComponent->RegisterComponent();
	SkeletalMeshComponent->FillComponent(InSkeletalMeshComponent);
}

void ULegacyWorldInfo::GetStreamingLevels(TArray<ULegacyLevelStreaming*>& OutStreamingLevels)
{
	OutStreamingLevels = StreamingLevels;
}

UClass* ULegacyWorldInfo::GetActorClass_Implementation()
{
	return nullptr;
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

void ULegacyActor::FillActor_Implementation(AActor* InActor)
{
}

UClass* ULegacyActor::GetActorClass_Implementation()
{
	return AActor::StaticClass();
}
