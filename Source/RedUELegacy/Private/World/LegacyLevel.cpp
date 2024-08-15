#include "World/LegacyLevel.h"

#include "Editor.h"
#include "Core/RedUELegacyArchive.h"
#include "Engine/LevelScriptBlueprint.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "World/Sequences/LegacySequence.h"

void ULegacyLevel::LegacySerialize(FRedUELegacyArchive& Ar)
{
    Super::LegacySerialize(Ar);
    ULegacyObject*ActorsOwner;
    Ar<<ActorsOwner;
    Ar<<Actors;
    Ar<<URL;
    Ar<<Model;
    Ar<<ModelComponents;
    Ar<<GameSequences;
    
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
    ALevelScriptActor* LevelScriptActor =  GWorld->GetLevelScriptActor();
    ULevelScriptBlueprint* LevelScriptBlueprint = GWorld->PersistentLevel->GetLevelScriptBlueprint(false);
    for(ULegacySequence* Sequence: GameSequences)
    {
        Sequence->GenerateBlueprint(LevelScriptBlueprint);
    }
    return nullptr;
}
