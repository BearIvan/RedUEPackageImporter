#pragma once

#include "Core/LegacyObject.h"
#include "Core/LegacyPackage.h"
#include "LegacyAnimSet.generated.h"

class ULegacyAnimSequence;

UCLASS()
class REDUELEGACY_API ULegacyAnimSet : public ULegacyObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Legacy|Editor",CallInEditor)
	void ImportSequences	(USkeleton* Skeleton);

	UFUNCTION(BlueprintCallable, Category = "Legacy|Editor",CallInEditor)
	void ReImportSequences	(USkeleton* Skeleton);
	
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

struct FLegacyMorphemeAnimRigToAnimMap
{
	int32 m_numRigBones;
	int32 m_numAnimChannels;
	int32 m_numEntries;
	int32 m_numCompToRigMapTablesPos;
	int32 m_numCompToRigMapTablesQuat;
	uint8 padding[4];
	void* m_entries;
	void* m_usedFlags;
	uint16* m_rigIndices;
	void Unpack();
};
static_assert(sizeof(FLegacyMorphemeAnimRigToAnimMap) == 0x30);

struct FLegacyMorphemeHierarchy
{
	int32	NumEntries;
	int32*	HierarchyArray;
	void	Unpack();
};

static_assert(sizeof(FLegacyMorphemeHierarchy) == 0x10);

struct FLegacyMorphemeAnimRigDef
{
	FQuat4f						BlendFrameOrientation;
	FVector3f					BlendFrameTranslation;
	FLegacyMorphemeHierarchy*	Hierarchy;
	int32						TrajectoryBoneIndex;
	int32						CharacterRootBoneIndex;
	void*						BoneNameMap;
	void*						BindPose;
	void						Unpack();
};

static_assert(sizeof(FLegacyMorphemeAnimRigDef) == 0x40);

UCLASS()
class REDUELEGACY_API ULegacyMorphemeAnimSet: public ULegacyAnimSet
{
	GENERATED_BODY()
public:
	virtual void 									LegacySerialize		(FRedUELegacyArchive& Ar) override;
			FLegacyMorphemeAnimRigToAnimMap*		GetAnimRigToAnimMap ();
			FLegacyMorphemeAnimRigDef*				GetAnimRigDef		();
	
			FRedUELegacyByteBulkData 				CompiledRigDataPC32;
			FRedUELegacyByteBulkData 				CompiledRigDataPC64;
			FRedUELegacyByteBulkData 				CompiledRigDataX360;
			FRedUELegacyByteBulkData 				CompiledRigDataPS3;
			FRedUELegacyByteBulkData 				CompiledRigToAnimMapDataPC32;
			FRedUELegacyByteBulkData 				CompiledRigToAnimMapDataPC64;
			FRedUELegacyByteBulkData 				CompiledRigToAnimMapDataX360;
			FRedUELegacyByteBulkData 				CompiledRigToAnimMapDataPS3;
			FLegacyMorphemeAnimRigToAnimMap*		CurrentAnimRigToAnimMap = nullptr;
			FLegacyMorphemeAnimRigDef*				CurrentAnimRigDef = nullptr;
};
	