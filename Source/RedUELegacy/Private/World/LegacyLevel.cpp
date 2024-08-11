#include "World/LegacyLevel.h"
#include "Core/RedUELegacyArchive.h"

void ULegacyLevel::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    ULegacyObject*ActorsOwner;
    Ar<<ActorsOwner;
    Ar<<Actors;
    Ar.Seek(Ar.GetStopper());
}

UObject* ULegacyLevel::ExportToContent()
{
    for(ULegacyActor* Actor:Actors)
    {
        if(Actor)
        {
            Actor->Spawn();
        }
    }
    return nullptr;
}
