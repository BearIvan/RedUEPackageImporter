
#include "World/Actors/LegacyStaticMeshCollectionActor.h"

#include "MaterialStatsCommon.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyStaticMeshCollectionActor::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    if (Ar.Game == ERedUELegacyGame::Bioshock3)
    {
        uint8 Unkown;
        Ar << Unkown;
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
            
        }
    }
    return nullptr;
}
