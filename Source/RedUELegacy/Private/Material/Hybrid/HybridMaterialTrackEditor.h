#pragma once
#include "Templates/SharedPointer.h"
#include "Templates/SubclassOf.h"
#include "TrackEditors/MaterialTrackEditor.h"
#include "TrackEditors/MaterialTrackEditor.h"

class AMaterialInstanceHybridActor;
class ISequencer;
class ISequencerTrackEditor;
class UMaterialInterface;
class UMovieSceneMaterialTrack;
class UMovieSceneTrack;
struct FGuid;

/**
 * A specialized material track editor for widget materials
 */
class FHybridMaterialTrackEditor
	: public FMaterialTrackEditor
{
public:

	FHybridMaterialTrackEditor( TSharedRef<ISequencer> InSequencer );

	static TSharedRef<ISequencerTrackEditor> CreateTrackEditor( TSharedRef<ISequencer> OwningSequencer );

public:

	// ISequencerTrackEditor interface

	virtual bool SupportsType( TSubclassOf<UMovieSceneTrack> Type ) const override;
	virtual void ExtendObjectBindingTrackMenu(TSharedRef<FExtender> Extender, const TArray<FGuid>& ObjectBindings, const UClass* ObjectClass) override;
	virtual bool GetDefaultExpansionState(UMovieSceneTrack* InTrack) const override;
	virtual void BuildTrackContextMenu(FMenuBuilder& MenuBuilder, UMovieSceneTrack* Track) override;

protected:

	void ConstructObjectBindingTrackMenu(FMenuBuilder& MenuBuilder, TArray<FGuid> ObjectBindings);
	/** Callback for executing the add component material track. */
	void HandleAddComponentMaterialActionExecute(AMaterialInstanceHybridActor* MaterialInstanceHybridActor);
	
	// FMaterialtrackEditor interface

	virtual UMaterialInterface* GetMaterialInterfaceForTrack( FGuid ObjectBinding, UMovieSceneMaterialTrack* MaterialTrack ) override;
};
