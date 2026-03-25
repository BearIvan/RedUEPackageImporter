#include "MovieSceneHybridMaterialSystem.h"

#include "MaterialInstanceHybrid.h"
#include "Actors/MaterialInstanceActor/MaterialInstanceHybridActor.h"
#include "Kismet/MovieSceneLegacyComponentTypes.h"
#include "Evaluation/PreAnimatedState/MovieScenePreAnimatedStorageID.inl"


namespace UE::MovieScene
{

FHybridMaterialAccessor::FHybridMaterialAccessor(const FHybridMaterialKey& InKey)
	: HybridActor(CastChecked<AMaterialInstanceHybridActor>(InKey.Object.ResolveObjectPtr(), ECastCheckedType::NullAllowed))
{
	if (HybridActor)
	{
		HybridMaterialHandle = InKey.HybridMaterialHandle;
	}
}

FHybridMaterialAccessor::FHybridMaterialAccessor(UObject* InObject, TObjectPtr<UMaterialInstanceHybrid> InHybridMaterialHandle)
	: HybridActor(Cast<AMaterialInstanceHybridActor>(InObject))
	, HybridMaterialHandle(MoveTemp(InHybridMaterialHandle))
{
	// Object must be a Hybrid
	if (InObject && !HybridActor)
	{
		// UE_LOG(LogUELegacy, Warning, TEXT("Cannot animate Hybrid material on object %s of type %s"),
		// 	*InObject->GetName(), *InObject->GetClass()->GetName()
		// );
	}
}

FHybridMaterialAccessor::operator bool() const
{
	return HybridActor != nullptr;
}

FString FHybridMaterialAccessor::ToString() const
{
	return FString::Printf(TEXT("on actor %s"), *HybridActor->GetPathName());
}

UMaterialInterface* FHybridMaterialAccessor::GetMaterial() const
{
	return HybridMaterialHandle.Get();
}

void FHybridMaterialAccessor::SetMaterial(UMaterialInterface* InMaterial)
{
	HybridMaterialHandle = Cast<UMaterialInstanceHybrid>(InMaterial);
}
UMaterialInstanceDynamic* FHybridMaterialAccessor::CreateDynamicMaterial(UMaterialInterface* InMaterial)
{
	SetMaterial(InMaterial);
	return HybridMaterialHandle.Get();
}

TAutoRegisterPreAnimatedStorageID<FPreAnimatedHybridMaterialSwitcherStorage> FPreAnimatedHybridMaterialSwitcherStorage::StorageID;
TAutoRegisterPreAnimatedStorageID<FPreAnimatedHybridMaterialParameterStorage> FPreAnimatedHybridMaterialParameterStorage::StorageID;

} // namespace UE::MovieScene

UMovieSceneHybridMaterialSystem::UMovieSceneHybridMaterialSystem(const FObjectInitializer& ObjInit)
	: Super(ObjInit)
{
	using namespace UE::MovieScene;

	FBuiltInComponentTypes* BuiltInComponents    = FBuiltInComponentTypes::Get();
	FMovieSceneLegacyComponentTypes* LegacyComponentTypes  = FMovieSceneLegacyComponentTypes::Get();
	FMovieSceneTracksComponentTypes* TracksComponents  = FMovieSceneTracksComponentTypes::Get();

	RelevantComponent = LegacyComponentTypes->HybridMaterialHandle;
	Phase = ESystemPhase::Instantiation;

	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		DefineComponentConsumer(GetClass(), BuiltInComponents->ObjectResult);
		DefineComponentConsumer(GetClass(), BuiltInComponents->BoundObject);
		DefineComponentProducer(GetClass(), TracksComponents->BoundMaterial);
		DefineImplicitPrerequisite(UMovieSceneCachePreAnimatedStateSystem::StaticClass(), GetClass());
	}
}

void UMovieSceneHybridMaterialSystem::OnLink()
{
	using namespace UE::MovieScene;

	FBuiltInComponentTypes*       BuiltInComponents = FBuiltInComponentTypes::Get();
	FMovieSceneLegacyComponentTypes* HybridComponents  = FMovieSceneLegacyComponentTypes::Get();

	SystemImpl.MaterialSwitcherStorage = Linker->PreAnimatedState.GetOrCreateStorage<FPreAnimatedHybridMaterialSwitcherStorage>();
	SystemImpl.MaterialParameterStorage = Linker->PreAnimatedState.GetOrCreateStorage<FPreAnimatedHybridMaterialParameterStorage>();

	SystemImpl.OnLink(Linker, BuiltInComponents->BoundObject, HybridComponents->HybridMaterialHandle);
}

void UMovieSceneHybridMaterialSystem::OnUnlink()
{
	SystemImpl.OnUnlink(Linker);
}

void UMovieSceneHybridMaterialSystem::OnRun(FSystemTaskPrerequisites& InPrerequisites, FSystemSubsequentTasks& Subsequents)
{
	using namespace UE::MovieScene;

	FBuiltInComponentTypes*       BuiltInComponents = FBuiltInComponentTypes::Get();
	FMovieSceneLegacyComponentTypes* HybridComponents  = FMovieSceneLegacyComponentTypes::Get();

	SystemImpl.OnRun(Linker, BuiltInComponents->BoundObject, HybridComponents->HybridMaterialHandle, InPrerequisites, Subsequents);
}

void UMovieSceneHybridMaterialSystem::SavePreAnimatedState(const FPreAnimationParameters& InParameters)
{
	using namespace UE::MovieScene;

	FBuiltInComponentTypes*       BuiltInComponents = FBuiltInComponentTypes::Get();
	FMovieSceneLegacyComponentTypes* HybridComponents  = FMovieSceneLegacyComponentTypes::Get();

	SystemImpl.SavePreAnimatedState(Linker, BuiltInComponents->BoundObject, HybridComponents->HybridMaterialHandle, InParameters);
}
