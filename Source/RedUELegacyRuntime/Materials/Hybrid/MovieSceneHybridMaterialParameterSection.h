#pragma once

#include "Sections/MovieSceneComponentMaterialParameterSection.h"
#include "MovieSceneHybridMaterialParameterSection.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API UMovieSceneHybridMaterialParameterSection : public UMovieSceneComponentMaterialParameterSection
{
	GENERATED_BODY()
public:
	virtual void ImportEntityImpl(UMovieSceneEntitySystemLinker* EntityLinker, const FEntityImportParams& Params, FImportedEntity* OutImportedEntity) override;
	
	virtual EMovieSceneChannelProxyType CacheChannelProxy() override;

	FMovieSceneFloatChannel* FindOrAddScalarParameterKey(const FName& ParametersName);
	void FindOrAddColorParameterKey(const FName& ParametersName,FMovieSceneFloatChannel*& RChannel,FMovieSceneFloatChannel*& GChannel, FMovieSceneFloatChannel*& BChannel, FMovieSceneFloatChannel*& AChannel);

};
