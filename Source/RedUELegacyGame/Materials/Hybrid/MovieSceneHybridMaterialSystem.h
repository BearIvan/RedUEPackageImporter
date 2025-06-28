#pragma once

#include "MovieSceneHybridMaterialTrack.h"
#include "EntitySystem/MovieSceneEntitySystem.h"
#include "EntitySystem/MovieScenePreAnimatedStateSystem.h"
#include "Systems/MovieSceneMaterialSystem.h"
#include "MovieSceneHybridMaterialSystem.generated.h"


class UMaterialInstanceHybrid;
class AMaterialInstanceHybridActor;

namespace UE::MovieScene
{

	struct FHybridMaterialKey
	{
		FObjectKey Object;
		TObjectPtr<UMaterialInstanceHybrid> HybridMaterialHandle;

		friend uint32 GetTypeHash(const FHybridMaterialKey& In)
		{
			return GetTypeHash(In.Object) ^ GetTypeHash(In.HybridMaterialHandle);
		}
		friend bool operator==(const FHybridMaterialKey& A, const FHybridMaterialKey& B)
		{
			return A.Object == B.Object && A.HybridMaterialHandle == B.HybridMaterialHandle;
		}
	};

	struct FHybridMaterialAccessor
	{
		using KeyType = FHybridMaterialKey;

		AMaterialInstanceHybridActor* HybridActor;
		TObjectPtr<UMaterialInstanceHybrid> HybridMaterialHandle;

		FHybridMaterialAccessor(const FHybridMaterialKey& InKey);
		FHybridMaterialAccessor(UObject* InObject, TObjectPtr<UMaterialInstanceHybrid> InHybridMaterialHandle);

		explicit operator bool() const;

		UMaterialInterface* GetMaterial() const;
		void SetMaterial(UMaterialInterface* InMaterial);
		UMaterialInstanceDynamic* CreateDynamicMaterial(UMaterialInterface* InMaterial);
		FString ToString() const;
	};

	using FPreAnimatedHybridMaterialTraits          = TPreAnimatedMaterialTraits<FHybridMaterialAccessor, UObject*, TObjectPtr<UMaterialInstanceHybrid>>;
	using FPreAnimatedHybridMaterialParameterTraits = TPreAnimatedMaterialParameterTraits<FHybridMaterialAccessor, UObject*, TObjectPtr<UMaterialInstanceHybrid>>;

	struct FPreAnimatedHybridMaterialSwitcherStorage
		: public TPreAnimatedStateStorage<TPreAnimatedMaterialTraits<FHybridMaterialAccessor, UObject*, TObjectPtr<UMaterialInstanceHybrid>>>
	{
		static TAutoRegisterPreAnimatedStorageID<FPreAnimatedHybridMaterialSwitcherStorage> StorageID;
	};

	struct FPreAnimatedHybridMaterialParameterStorage
		: public TPreAnimatedStateStorage<TPreAnimatedMaterialParameterTraits<FHybridMaterialAccessor, UObject*, TObjectPtr<UMaterialInstanceHybrid>>>
	{
		static TAutoRegisterPreAnimatedStorageID<FPreAnimatedHybridMaterialParameterStorage> StorageID;
	};

} // namespace UE::MovieScene



UCLASS()
class REDUELEGACYGAME_API UMovieSceneHybridMaterialSystem
	: public UMovieSceneEntitySystem
	, public IMovieScenePreAnimatedStateSystemInterface
{
public:

	GENERATED_BODY()

	UMovieSceneHybridMaterialSystem(const FObjectInitializer& ObjInit);

private:

	virtual void OnLink() override;
	virtual void OnUnlink() override;
	virtual void OnRun(FSystemTaskPrerequisites& InPrerequisites, FSystemSubsequentTasks& Subsequents) override;

	virtual void SavePreAnimatedState(const FPreAnimationParameters& InParameters) override;

private:

	UE::MovieScene::TMovieSceneMaterialSystem<UE::MovieScene::FHybridMaterialAccessor, UObject*,  TObjectPtr<UMaterialInstanceHybrid>> SystemImpl;
};
