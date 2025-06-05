#include "World/LegacyLevel.h"

#include "Editor.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"
#include "Engine/LevelScriptBlueprint.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "World/LegacyWorld.h"
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
    return nullptr;
}

void ULegacyLevel::ImportLevel(bool ReimportKismet)
{
    URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
    for(ULegacyActor* Actor:Actors)
    {
        if(Actor)
        {
            Actor->Spawn();
        }
    }
    for(ULegacyActor* Actor:Actors)
    {
        if(Actor)
        {
            if (Actor->Base)
            {
                AActor* Parent =  CastChecked<AActor>(Actor->Base->PresentObject,ECastCheckedType::NullAllowed);
                AActor* Me =  CastChecked<AActor>(Actor->PresentObject,ECastCheckedType::NullAllowed);
                if (Parent && Me)
                {
                    Me->AttachToActor(Parent,FAttachmentTransformRules::KeepWorldTransform);
                }
            }
        }
    }
    ALevelScriptActor* LevelScriptActor =  GWorld->GetLevelScriptActor();
    ULevelScriptBlueprint* LevelScriptBlueprint = GWorld->PersistentLevel->GetLevelScriptBlueprint(false);
    for(ULegacySequence* Sequence: GameSequences)
    {
        if (UBlueprint* KismetBlueprint = CastChecked<UBlueprint>(Sequence->ImportKismet(ReimportKismet),ECastCheckedType::NullAllowed))
        {
            ALegacyKismet*LevelKismet = GWorld->SpawnActor<ALegacyKismet>(KismetBlueprint->GeneratedClass);
            Sequence->FillActor(LevelKismet);
            
        }
    }
}
