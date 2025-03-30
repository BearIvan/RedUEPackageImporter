#include "World/Actors/LegacyStaticMeshActor.h"
#include "World/Components/LegacyStaticMeshComponent.h"

void ULegacyStaticMeshActor::Spawn_Implementation()
{
    Super::Spawn_Implementation();
    AStaticMeshActor* StaticMeshActor = GWorld->SpawnActor<AStaticMeshActor>(FVector(Translation),FRotator(Rotation));
    StaticMeshActor->SetActorScale3D(FVector(Scale3D)*DrawScale);
    if(StaticMeshComponent&&StaticMeshComponent->StaticMesh)
    {
        if(UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(StaticMeshComponent->StaticMesh->ExportToContent(),ECastCheckedType::NullAllowed))
        {
            StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
            for(int32  i = 0; i < StaticMeshComponent->Materials.Num();i++)
            {
                if(StaticMeshComponent->Materials[i])
                {
                    StaticMeshActor->GetStaticMeshComponent()->SetMaterial(i,CastChecked<UMaterialInterface>(StaticMeshComponent->Materials[i]->ExportToContent(),ECastCheckedType::NullAllowed));
                }
            }
        }
        PresentObject = StaticMeshActor;
    }
}
