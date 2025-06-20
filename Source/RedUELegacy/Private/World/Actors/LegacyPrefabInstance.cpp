﻿#include "World/Actors/LegacyPrefabInstance.h"
#include "Core/RedUELegacyArchive.h"
#include "World/Actors/LegacyStaticMeshActor.h"
#include "World/Components/LegacyStaticMeshComponent.h"

void ULegacyPrefabInstance::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar<<ArchetypeToInstanceMap;
    Ar.Seek(Ar.GetStopper());
}

AActor* ULegacyPrefabInstance::Spawn_Implementation()
{
    return nullptr;
}

void ULegacyPrefabInstance::LegacyPostLoad()
{
    Super::LegacyPostLoad();
    for(auto&[Key,Value]:ArchetypeToInstanceMap)
    {
        if(const ULegacyStaticMeshActor* TemplateLegacyStaticMeshActor = Cast<ULegacyStaticMeshActor>(Key))
        {
            const ULegacyStaticMeshActor* LevelLegacyStaticMeshActor = CastChecked<ULegacyStaticMeshActor>(Value);
            LevelLegacyStaticMeshActor->StaticMeshComponent->StaticMesh = TemplateLegacyStaticMeshActor->StaticMeshComponent->StaticMesh;
            LevelLegacyStaticMeshActor->StaticMeshComponent->Materials = TemplateLegacyStaticMeshActor->StaticMeshComponent->Materials;
        }
    }
}
