#include "HybridMaterialTrackEditor.h"

#include "Editor.h"
#include "Containers/ArrayView.h"
#include "ISequencer.h"
#include "ISequencerModule.h"
#include "Actors/MaterialInstanceActor/MaterialInstanceHybridActor.h"
#include "Materials/Hybrid/MaterialInstanceHybrid.h"
#include "Materials/Hybrid/MovieSceneHybridMaterialTrack.h"
#include "Misc/Guid.h"
#include "Templates/Casts.h"
#include "Templates/UnrealTemplate.h"
#include "Tracks/MovieSceneMaterialTrack.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#define LOCTEXT_NAMESPACE "RedUELegacy"

class ISequencerTrackEditor;
class UMovieSceneTrack;


FHybridMaterialTrackEditor::FHybridMaterialTrackEditor( TSharedRef<ISequencer> InSequencer )
	: FMaterialTrackEditor( InSequencer )
{
}


TSharedRef<ISequencerTrackEditor> FHybridMaterialTrackEditor::CreateTrackEditor( TSharedRef<ISequencer> OwningSequencer )
{
	return MakeShareable( new FHybridMaterialTrackEditor( OwningSequencer ) );
}


bool FHybridMaterialTrackEditor::SupportsType( TSubclassOf<UMovieSceneTrack> Type ) const
{
	return Type == UMovieSceneHybridMaterialTrack::StaticClass();
}


UMaterialInterface* FHybridMaterialTrackEditor::GetMaterialInterfaceForTrack( FGuid ObjectBinding, UMovieSceneMaterialTrack* MaterialTrack )
{
	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	if (!SequencerPtr.IsValid())
	{
		return nullptr;
	}

	UMovieSceneHybridMaterialTrack* HybridMaterialTrack = Cast<UMovieSceneHybridMaterialTrack>( MaterialTrack );
	if (!HybridMaterialTrack)
	{
		return nullptr;
	}

	UObject* Object = GetSequencer()->FindSpawnedObjectOrTemplate(ObjectBinding);
	if (!Object)
	{
		return nullptr;
	}
	if (AMaterialInstanceHybridActor* MaterialInstanceHybridActor = Cast<AMaterialInstanceHybridActor>(Object))
	{
		return MaterialInstanceHybridActor->MaterialInstance;
	}
	return nullptr;
}



bool FHybridMaterialTrackEditor::GetDefaultExpansionState(UMovieSceneTrack* InTrack) const
{
	return true;
}

void FHybridMaterialTrackEditor::BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track)
{
	if (UMovieSceneHybridMaterialTrack* MaterialTrack = Cast<UMovieSceneHybridMaterialTrack>(Track))
	{
		FGuid ObjectBinding = MaterialTrack->FindObjectBindingGuid();
		UObject* BoundObject = GetSequencer()->FindSpawnedObjectOrTemplate(ObjectBinding);
		const TWeakObjectPtr<UMaterialInstanceHybrid>& MaterialHybrid = MaterialTrack->GetMaterialHybrid();
	}
}


void FHybridMaterialTrackEditor::ExtendObjectBindingTrackMenu(TSharedRef<FExtender> Extender, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass)
{
	if (ObjectClass->IsChildOf(AMaterialInstanceHybridActor::StaticClass()))
	{
		Extender->AddMenuExtension(SequencerMenuExtensionPoints::AddTrackMenu_PropertiesSection, EExtensionHook::Before, nullptr, FMenuExtensionDelegate::CreateSP(this, &FHybridMaterialTrackEditor::ConstructObjectBindingTrackMenu, ObjectBindings));
	}
}

void FHybridMaterialTrackEditor::ConstructObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, TArray<FGuid> ObjectBindings)
{
	auto GetHybridMaterialForTrack = [](UMovieSceneTrack* InTrack)
	{
		UMovieSceneHybridMaterialTrack* MaterialTrack = Cast<UMovieSceneHybridMaterialTrack>(InTrack);
		return MaterialTrack != nullptr;
	};
	
	UObject* Object = GetSequencer()->FindSpawnedObjectOrTemplate(ObjectBindings[0]);
	if (!Object)
	{
		return;
	}

	AMaterialInstanceHybridActor* MaterialInstanceHybridActor = Cast<AMaterialInstanceHybridActor>(Object);
	if (!MaterialInstanceHybridActor)
	{
		return;
	}

	const UMovieScene* MovieScene = GetFocusedMovieScene();
	const FMovieSceneBinding* Binding = MovieScene->FindBinding(ObjectBindings[0]);

	{
		if (UMaterialInterface* DecalMaterial = MaterialInstanceHybridActor->MaterialInstance)
		{
			MenuBuilder.BeginSection("Materials", LOCTEXT("MaterialSection", "Material Parameters"));
			{
				FComponentMaterialInfo MaterialInfo{ FName(), 0, EComponentMaterialType::DecalMaterial};
				const bool bAlreadyExists =  Binding->GetTracks().ContainsByPredicate(GetHybridMaterialForTrack);
				if (!bAlreadyExists)
				{
					FUIAction AddComponentMaterialAction(FExecuteAction::CreateRaw(this, &FHybridMaterialTrackEditor::HandleAddComponentMaterialActionExecute, MaterialInstanceHybridActor));
					FText AddDecalMaterialLabel = FText::Format(LOCTEXT("AddHybridMaterialLabelFormat", "Hybrid material: {0}"), FText::FromString(DecalMaterial->GetName()));
					FText AddDecalMaterialToolTip = FText::Format(LOCTEXT("AddHybridMaterialToolTipFormat", "Add hybrid material {0}"), FText::FromString(DecalMaterial->GetName()));
					MenuBuilder.AddMenuEntry(AddDecalMaterialLabel, AddDecalMaterialToolTip, FSlateIcon(), AddComponentMaterialAction);
				}
			}
			MenuBuilder.EndSection();
		}
	}
}

void FHybridMaterialTrackEditor::HandleAddComponentMaterialActionExecute(AMaterialInstanceHybridActor* MaterialInstanceHybridActor)
{
	auto GetHybridMaterialForTrack = [](UMovieSceneTrack* InTrack)
	{
		UMovieSceneHybridMaterialTrack* MaterialTrack = Cast<UMovieSceneHybridMaterialTrack>(InTrack);
		return MaterialTrack != nullptr;
	};
	TSharedPtr<ISequencer> SequencerPtr = GetSequencer();
	UMovieScene* MovieScene = SequencerPtr->GetFocusedMovieSceneSequence()->GetMovieScene();
	if (MovieScene->IsReadOnly())
	{
		return;
	}

	const FScopedTransaction Transaction(LOCTEXT("AddComponentMaterialTrack", "Add component hybrid material track"));

	MovieScene->Modify();
	{
		FGuid ObjectHandle = SequencerPtr->GetHandleToObject(MaterialInstanceHybridActor);
		const FMovieSceneBinding* Binding = MovieScene->FindBinding(ObjectHandle);
		const bool bAlreadyExists =  Binding->GetTracks().ContainsByPredicate(GetHybridMaterialForTrack);
		if (!bAlreadyExists)
		{
			UMovieSceneHybridMaterialTrack* MaterialTrack = MovieScene->AddTrack<UMovieSceneHybridMaterialTrack>(ObjectHandle);
			MaterialTrack->SetMaterialHybrid(MaterialInstanceHybridActor->MaterialInstance);
			MaterialTrack->Modify();
			// Construct display name from MaterialInfo
			UMaterialInterface* MaterialInterface = GetMaterialInterfaceForTrack(ObjectHandle, MaterialTrack);

			FText TrackDisplayName = FText::Format(LOCTEXT("HybridMaterialTrackName", "Hybrid Material {0}"), MaterialInterface ? FText::FromString(MaterialInterface->GetName()) : FText());
			if (!TrackDisplayName.IsEmpty())
			{
				MaterialTrack->SetDisplayName(TrackDisplayName);
			}
		}
	}

	SequencerPtr->NotifyMovieSceneDataChanged(EMovieSceneDataChangeType::MovieSceneStructureItemAdded);
}
#undef LOCTEXT_NAMESPACE