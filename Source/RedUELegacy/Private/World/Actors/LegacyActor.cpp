#include "World/Actors/LegacyActor.h"

#include "Core/RedUELegacyArchive.h"
#include "World/Components/LegacySkeletalMeshComponent.h"

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

UClass* ULegacyCameraActor::GetActorClass_Implementation()
{
	return ACameraActor::StaticClass();
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

void ULegacyActor::FillActor_Implementation(AActor* InActor)
{
}

UClass* ULegacyActor::GetActorClass_Implementation()
{
	return AActor::StaticClass();
}
