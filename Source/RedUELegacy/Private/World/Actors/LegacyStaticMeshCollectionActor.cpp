
#include "World/Actors/LegacyStaticMeshCollectionActor.h"

#include "Core/RedUELegacyArchive.h"

void ULegacyStaticMeshCollectionActor::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
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

void ULegacyStaticMeshCollectionActor::Spawn_Implementation()
{
    Super::Spawn_Implementation();
    for (const ULegacyStaticMeshComponent*Component:StaticMeshComponents)
    {
        if(Component&&Component->StaticMesh)
        {
            FString Test = Component->GetLegacyFullName();
            if(UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(Component->StaticMesh->ExportToContent(),ECastCheckedType::NullAllowed))
            {
                AStaticMeshActor* StaticMeshActor = GWorld->SpawnActor<AStaticMeshActor>(FVector(Component->Translation),FRotator(Component->Rotation));
                StaticMeshActor->SetActorScale3D(FVector(Component->Scale3D));
                StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
                for(int32  i = 0; i < Component->Materials.Num();i++)
                {
                    if(Component->Materials[i])
                    {
                        StaticMeshActor->GetStaticMeshComponent()->SetMaterial(i,CastChecked<UMaterialInterface>(Component->Materials[i]->ExportToContent(),ECastCheckedType::NullAllowed));
                    }
                }
            }
        }
    }
}
