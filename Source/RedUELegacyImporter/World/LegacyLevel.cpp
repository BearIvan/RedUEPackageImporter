#include "World/LegacyLevel.h"
#include "Editor.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"
#include "Entities/LegacyActor.h"
#include "Kismet/Base/LegacyKismet.h"
#include "Mesh/LegacyModel.h"
#include "Sequence/LegacySequence.h"

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
                    FName SocketName = NAME_None;
                    // if (Parent->IsA<AXMatineeCameraActor>())
                    // {
                    //     SocketName = "CameraDirect";
                    // }
                    Me->AttachToActor(Parent,FAttachmentTransformRules::KeepWorldTransform,SocketName);
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
    if (Model)
    {
        ABrush*BrushActor = GWorld->SpawnActor<ABrush>();
        Model->Fill(BrushActor);
    }
}
