#pragma once
#include "Entities/Primitive/LegacyPrimitiveComponent.h"
#include "LegacySkeletalMeshComponent.generated.h"

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySkeletalMeshComponent : public ULegacyMeshComponent
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Legacy|Editor",CallInEditor)
	UAnimSequenceBase* GetAnimSequence(FName AnimName);
		
	virtual void FillComponent_Implementation(UActorComponent* InActorComponent) override;
	
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* SkeletalMesh;

	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacyMaterialInterface*>	Materials;

	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacyAnimSet*> AnimSets;

	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXSkeletalMeshComponent : public ULegacySkeletalMeshComponent
{
	GENERATED_BODY()
public:
};