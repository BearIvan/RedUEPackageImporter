#pragma once

#include "Tracks/MovieSceneMaterialTrack.h"
#include "MovieSceneHybridMaterialTrack.generated.h"
class UMaterialInstanceHybrid;
UCLASS()
class REDUELEGACYRUNTIME_API UMovieSceneHybridMaterialTrack : public UMovieSceneMaterialTrack
	, public IMovieSceneEntityProvider
	, public IMovieSceneParameterSectionExtender
{
	GENERATED_BODY()
public:
	UMovieSceneHybridMaterialTrack(const FObjectInitializer& ObjectInitializer);
	
	 virtual bool SupportsType(TSubclassOf<UMovieSceneSection> SectionClass) const override;
	 virtual UMovieSceneSection* CreateNewSection() override;
	/*~ IMovieSceneEntityProvider */
	virtual void ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity) override;
	virtual bool PopulateEvaluationFieldImpl(const TRange<FFrameNumber>& EffectiveRange, const FMovieSceneEvaluationFieldEntityMetaData& InMetaData, FMovieSceneEntityComponentFieldBuilder* OutFieldBuilder) override;

	/*~ IMovieSceneParameterSectionExtender */
	virtual void ExtendEntityImpl(UMovieSceneParameterSection* Section, UMovieSceneEntitySystemLinker* EntityLinker, const UE::MovieScene::FEntityImportParams& Params, UE::MovieScene::FImportedEntity* OutImportedEntity) override;


#if WITH_EDITORONLY_DATA
	virtual FText GetDefaultDisplayName() const override;
#endif
public:

	
	/** Gets the index of the material in the component. */
	const TObjectPtr<UMaterialInstanceHybrid>& GetMaterialHybrid() const { return MaterialHybrid; }

	/** Sets the index of the material in the component. */
	void SetMaterialHybrid(const TObjectPtr<UMaterialInstanceHybrid>& InMaterialHybrid)
	{
		MaterialHybrid = InMaterialHybrid;
	}
	
#if WITH_EDITOR
	virtual FText GetDisplayNameToolTipText(const FMovieSceneLabelParams& LabelParams) const override;

	// We override label color if material binding is broken/partially broken.
	virtual FSlateColor GetLabelColor(const FMovieSceneLabelParams& LabelParams) const override;
#endif


#if WITH_EDITORONLY_DATA

protected:
	void PostLoad() override;

private:
	/** The index of this material this track is animating. Has been deprecated in favor of MaterialInfo*/
	UPROPERTY(meta = (DeprecatedProperty, DeprecationMessage = "Use MaterialInfo instead."))
	int32 MaterialIndex_DEPRECATED;

#endif

	UPROPERTY()
	TObjectPtr<UMaterialInstanceHybrid> MaterialHybrid;
};
