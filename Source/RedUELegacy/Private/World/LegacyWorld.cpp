#include "World/LegacyWorld.h"

#include "Core/RedUELegacyArchive.h"

void ULegacyWorld::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    Ar << PersistentLevel;
    Ar.Seek(Ar.GetStopper());
}

UObject* ULegacyWorld::ExportToContent()
{
    PersistentLevel->ExportToContent();
    return nullptr;
}
