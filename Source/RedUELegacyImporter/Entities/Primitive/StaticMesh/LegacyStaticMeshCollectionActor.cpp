#include "LegacyStaticMeshCollectionActor.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyStaticMeshCollectionActor::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	if (Ar.Game == ERedUELegacyGame::Bioshock3)
	{
		Ar << BioCollisionType;
	}
	TArray<FMatrix44f> Matrices;
	Matrices.AddZeroed(StaticMeshComponents.Num());
	for (int32 i = 0; i < StaticMeshComponents.Num(); i++)
	{
		Ar << Matrices[i];
		if (StaticMeshComponents[i])
		{
			ULegacyStaticMeshComponent* StaticMeshComponent = StaticMeshComponents[i];
			StaticMeshComponent->Rotation = Matrices[i].Rotator();
			StaticMeshComponent->Translation = Matrices[i].GetOrigin();
		}
	}
}

AActor* ULegacyStaticMeshCollectionActor::Spawn_Implementation()
{
	for (ULegacyStaticMeshComponent*Component:StaticMeshComponents)
	{
		if(Component&&Component->StaticMesh)
		{
			AStaticMeshActor* StaticMeshActor = GWorld->SpawnActor<AStaticMeshActor>(FVector(Component->Translation),Component->Rotation);
			StaticMeshActor->SetActorScale3D(FVector(Component->Scale3D));
			Component->FillComponent(StaticMeshActor->GetStaticMeshComponent());

			static FName NAME_OverlapAll = "OverlapAll";
			switch (BioCollisionType)
			{
			case ELegacyBioshockCollisionType::COLLIDE_NoCollision:
			case ELegacyBioshockCollisionType::COLLIDE_DecalsOnly:
				StaticMeshActor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
				break;
			case ELegacyBioshockCollisionType::COLLIDE_BlockAll:
			case ELegacyBioshockCollisionType::COLLIDE_BlockWeapons:
			case ELegacyBioshockCollisionType::COLLIDE_BlockAllButWeapons:
			case ELegacyBioshockCollisionType::COLLIDE_BlockWeaponsKickable:
			case ELegacyBioshockCollisionType::COLLIDE_BlockAllButRigidBody:
				StaticMeshActor->GetStaticMeshComponent()->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
				break;
			case ELegacyBioshockCollisionType::COLLIDE_TouchWeapons:
			case ELegacyBioshockCollisionType::COLLIDE_TouchAll:
			case ELegacyBioshockCollisionType::COLLIDE_TouchAllButWeapons:
				StaticMeshActor->GetStaticMeshComponent()->SetCollisionProfileName(NAME_OverlapAll);
				break;
			default: ;
			}
		}
	}
	return nullptr;
}
