#include "MovieSceneHybridMaterialTrack.h"

#include "IMovieScenePlayer.h"
#include "MaterialInstanceHybrid.h"
#include "MovieSceneHybridMaterialParameterSection.h"
#include "MovieSceneTracksComponentTypes.h"
#include "Actors/MaterialInstanceActor/MaterialInstanceHybridActor.h"
#include "EntitySystem/BuiltInComponentTypes.h"
#include "Kismet/MovieSceneLegacyComponentTypes.h"
#include "Sections/MovieSceneComponentMaterialParameterSection.h"

#define LOCTEXT_NAMESPACE "RedUELegacy"



UMovieSceneHybridMaterialTrack::UMovieSceneHybridMaterialTrack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	BuiltInTreePopulationMode = ETreePopulationMode::Blended;
}

bool UMovieSceneHybridMaterialTrack::SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const
{
	return SectionClass == UMovieSceneHybridMaterialParameterSection::StaticClass() ||  Super::SupportsType(SectionClass);
}

UMovieSceneSection* UMovieSceneHybridMaterialTrack::CreateNewSection()
{
	UMovieSceneSection* NewSection = NewObject<UMovieSceneHybridMaterialParameterSection>(this, NAME_None, RF_Transactional);
	NewSection->SetBlendType(EMovieSceneBlendType::Absolute);
	return NewSection;
}

void UMovieSceneHybridMaterialTrack::ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity)
{
	// These tracks don't define any entities for themselves
	checkf(false, TEXT("This track should never have created entities for itself - this assertion indicates an error in the entity-component field"));
}

void UMovieSceneHybridMaterialTrack::ExtendEntityImpl(UMovieSceneParameterSection* Section, UMovieSceneEntitySystemLinker* EntityLinker, const UE::MovieScene::FEntityImportParams& Params, UE::MovieScene::FImportedEntity* OutImportedEntity)
{
	using namespace UE::MovieScene;

	FBuiltInComponentTypes* BuiltInComponents = FBuiltInComponentTypes::Get();
	FMovieSceneLegacyComponentTypes* LegacyComponentTypes = FMovieSceneLegacyComponentTypes::Get();

	// Material parameters are always absolute blends for the time being
	OutImportedEntity->AddBuilder(
		FEntityBuilder()
		.Add(LegacyComponentTypes->HybridMaterialHandle, MaterialHybrid)
		// If the section has no valid blend type (legacy data), make it use absolute blending.
		// Otherwise, the base section class will add the appropriate blend type tag in BuildDefaultComponents.
		.AddTagConditional(BuiltInComponents->Tags.AbsoluteBlend, !Section->GetBlendType().IsValid())
	);
}

bool UMovieSceneHybridMaterialTrack::PopulateEvaluationFieldImpl(const TRange<FFrameNumber>& EffectiveRange, const FMovieSceneEvaluationFieldEntityMetaData& InMetaData, FMovieSceneEntityComponentFieldBuilder* OutFieldBuilder)
{
	const FMovieSceneTrackEvaluationField& LocalEvaluationField = GetEvaluationField();

	// Define entities for the old style parameter sections. ComponentMaterialParameterSections define their own.
	for (const FMovieSceneTrackEvaluationFieldEntry& Entry : LocalEvaluationField.Entries)
	{
		UMovieSceneParameterSection* ParameterSection = Cast<UMovieSceneParameterSection>(Entry.Section);
		UMovieSceneComponentMaterialParameterSection* ComponentMaterialParameterSection = Cast<UMovieSceneComponentMaterialParameterSection>(Entry.Section);
		
		if (ParameterSection||ComponentMaterialParameterSection)
		{
			if (IsRowEvalDisabled(Entry.Section->GetRowIndex()))
			{
				continue;
			}

			TRange<FFrameNumber> SectionEffectiveRange = TRange<FFrameNumber>::Intersection(EffectiveRange, Entry.Range);
			if (!SectionEffectiveRange.IsEmpty())
			{
				FMovieSceneEvaluationFieldEntityMetaData SectionMetaData = InMetaData;
				SectionMetaData.Flags = Entry.Flags;
				if (ParameterSection)
				{
					ParameterSection->ExternalPopulateEvaluationField(SectionEffectiveRange, SectionMetaData, OutFieldBuilder);
				}
				else if (ComponentMaterialParameterSection)
				{
					ComponentMaterialParameterSection->ExternalPopulateEvaluationField(SectionEffectiveRange, SectionMetaData, OutFieldBuilder);
				}
			}
		}
	}

	return true;
}


#if WITH_EDITORONLY_DATA

FText UMovieSceneHybridMaterialTrack::GetDefaultDisplayName() const
{
	// Old track name before we started naming directly from editor
	return FText::FromString(FString::Printf(TEXT("Material Element %i"), MaterialIndex_DEPRECATED));
}

FText UMovieSceneHybridMaterialTrack::GetDisplayNameToolTipText(const FMovieSceneLabelParams& LabelParams) const
{
	if (LabelParams.Player)
	{
		UMaterialInterface* Material = nullptr;
		for (TWeakObjectPtr<> WeakObject : LabelParams.Player->FindBoundObjects(LabelParams.BindingID, LabelParams.SequenceID))
		{
			if (AMaterialInstanceHybridActor* HybridActor = Cast<AMaterialInstanceHybridActor>(WeakObject.Get()))
			{
				Material = HybridActor->MaterialInstance;
				if (Material)
				{
					return FText::Format(LOCTEXT("OverlayMaterialTrackTooltip", "Material parameter track for overlay material {0}"), FText::FromString(Material->GetName()));
				}
				else
				{
					return LOCTEXT("OverlayMaterialTrackTooltip_MissingMaterial", "No overlay material could be found");
				}
			}
		}
	}
	return FText();
}

FSlateColor UMovieSceneHybridMaterialTrack::GetLabelColor(const FMovieSceneLabelParams& LabelParams) const
{
	auto GetDimmedColor = [&LabelParams](FLinearColor LinearColor)
	{
		return FSlateColor(LabelParams.bIsDimmed ? LinearColor.Desaturate(0.6f) : LinearColor);
	};
	if (LabelParams.Player)
	{
		for (TWeakObjectPtr<> WeakObject : LabelParams.Player->FindBoundObjects(LabelParams.BindingID, LabelParams.SequenceID))
		{
			if (AMaterialInstanceHybridActor* HybridActor = Cast<AMaterialInstanceHybridActor>(WeakObject.Get()))
			{
				UMaterialInterface* Material = HybridActor->MaterialInstance;
				if (!Material)
				{
					return GetDimmedColor(FLinearColor::Green);
				}
			}
			else
			{
				return GetDimmedColor(FLinearColor::Red);
			}
		}
	}
	return LabelParams.bIsDimmed ? FSlateColor::UseSubduedForeground() : FSlateColor::UseForeground();
}

void UMovieSceneHybridMaterialTrack::PostLoad()
{
	Super::PostLoad();
}
#endif
#undef  LOCTEXT_NAMESPACE