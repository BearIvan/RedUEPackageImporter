#pragma once

#include "Core/LegacyObject.h"
#include "LegacyAnimSet.generated.h"

class ULegacyAnimSequence;

UCLASS()
class REDUELEGACY_API ULegacyAnimSet : public ULegacyObject
{
	GENERATED_BODY()
public:
	
	void ImportSequences	(USkeleton* Skeleton);
	
	UPROPERTY(BlueprintReadWrite)
	bool bAnimRotationOnly;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> TrackBoneNames;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyAnimSequence*> Sequences;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> UseTranslationBoneNames;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> ForceMeshTranslationBoneNames;

	UPROPERTY(BlueprintReadWrite)
	FName PreviewSkelMeshName;

	UPROPERTY(Transient)
	TMap<ULegacyAnimSequence*,UAnimSequence*> ImportedSequences;
};
