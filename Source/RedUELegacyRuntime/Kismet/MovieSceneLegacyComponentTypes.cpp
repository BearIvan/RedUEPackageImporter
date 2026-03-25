#include "MovieSceneLegacyComponentTypes.h"

#include "EntitySystem/BuiltInComponentTypes.h"
#include "EntitySystem/MovieSceneEntitySystemLinker.h"
#include "EntitySystem/MovieSceneEntityFactoryTemplates.h"


static bool GMovieSceneLegacyComponentTypesDestroyed = false;
static TUniquePtr<FMovieSceneLegacyComponentTypes> GMovieSceneLegacyComponentTypes;

void FMovieSceneLegacyComponentTypes::Destroy()
{
	GMovieSceneLegacyComponentTypesDestroyed = false;
	GMovieSceneLegacyComponentTypes.Reset();
}

FMovieSceneLegacyComponentTypes* FMovieSceneLegacyComponentTypes::Get()
{
	if (!GMovieSceneLegacyComponentTypes.IsValid())
	{
		check(!GMovieSceneLegacyComponentTypesDestroyed);
		GMovieSceneLegacyComponentTypes.Reset(new FMovieSceneLegacyComponentTypes);
	}
	return GMovieSceneLegacyComponentTypes.Get();
}

FMovieSceneLegacyComponentTypes::FMovieSceneLegacyComponentTypes()
{
	using namespace 	UE::MovieScene;
	
	//FBuiltInComponentTypes* BuiltInComponents = FBuiltInComponentTypes::Get();
	FComponentRegistry*     ComponentRegistry = UMovieSceneEntitySystemLinker::GetComponents();
	ComponentRegistry->NewComponentType(&HybridMaterialHandle, TEXT("Hybrid Material Handle"), EComponentTypeFlags::CopyToChildren | EComponentTypeFlags::CopyToOutput);

}
