#include "World/Actors/LegacyStaticMeshActor.h"
#include "World/Components/LegacyStaticMeshComponent.h"

void ULegacyStaticMeshActor::Spawn_Implementation()
{
    Super::Spawn_Implementation();
    if(StaticMeshComponent&&StaticMeshComponent->StaticMesh)
    {
        if(UStaticMesh* StaticMesh = CastChecked<UStaticMesh>(StaticMeshComponent->StaticMesh->ExportToContent(),ECastCheckedType::NullAllowed))
        {
            AStaticMeshActor* StaticMeshActor = GWorld->SpawnActor<AStaticMeshActor>(FVector(StaticMeshComponent->Translation),FRotator(StaticMeshComponent->Rotation));
            StaticMeshActor->SetActorScale3D(FVector(StaticMeshComponent->Scale3D));
            StaticMeshActor->GetStaticMeshComponent()->SetStaticMesh(StaticMesh);
            for(int32  i = 0; i < StaticMeshComponent->Materials.Num();i++)
            {
                if(StaticMeshComponent->Materials[i])
                {
                    StaticMeshActor->GetStaticMeshComponent()->SetMaterial(i,CastChecked<UMaterialInterface>(StaticMeshComponent->Materials[i]->ExportToContent(),ECastCheckedType::NullAllowed));
                }
            }
        }
    }
}
