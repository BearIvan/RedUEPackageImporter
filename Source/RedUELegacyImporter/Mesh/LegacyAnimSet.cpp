#include "Mesh/LegacyAnimSet.h"

#include "Animation/AnimSequence.h"
#include "Mesh/LegacyAnimSequence.h"

void ULegacyAnimSet::ImportSequences(USkeleton* Skeleton)
{
	if (Skeleton == nullptr)
	{
		return;
	}
	if (ImportedSequences.IsEmpty())
	{
		for (ULegacyAnimSequence* Sequence : Sequences)
		{
			if (Sequence)
			{
				ImportedSequences.Add(Sequence,Sequence->CreateSequence(this,Skeleton));
			}
		}
	}
}

bool ULegacyAnimSet::ContainsSequence(FName AnimName) const
{
	for (ULegacyAnimSequence* Sequence : Sequences)
	{
		if (Sequence->SequenceName == AnimName)
		{
			return true;
		}
	}
	return false;
}

UAnimSequenceBase* ULegacyAnimSet::FindOrImportSequence(USkeleton* Skeleton, FName AnimName)
{
	for (ULegacyAnimSequence* Sequence : Sequences)
	{
		if (Sequence->SequenceName == AnimName)
		{
			UAnimSequence** ImportedAnim = ImportedSequences.Find(Sequence);
			if (!ImportedAnim)
			{
				ImportSequences(Skeleton);
			}
			ImportedAnim = ImportedSequences.Find(Sequence);
			if (ImportedAnim)
			{
				return *ImportedAnim;
			}
		}
	}
	return nullptr;
}


void ULegacyAnimSet::ReImportSequences(USkeleton* Skeleton)
{
	ImportedSequences.Empty();
	ImportSequences(Skeleton);
}

void FLegacyMorphemeAnimRigToAnimMap::Unpack()
{
	m_rigIndices = reinterpret_cast<uint16*>(reinterpret_cast<uint8*>(this) + reinterpret_cast<int64>(m_rigIndices));
}

void ULegacyMorphemeAnimSet::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	CompiledRigDataPC32.Serialize(Ar);
	CompiledRigDataPC64.Serialize(Ar);
	CompiledRigDataX360.Serialize(Ar);
	CompiledRigDataPS3.Serialize(Ar);
	CompiledRigToAnimMapDataPC32.Serialize(Ar);
	CompiledRigToAnimMapDataPC64.Serialize(Ar);
	CompiledRigToAnimMapDataX360.Serialize(Ar);
	CompiledRigToAnimMapDataPS3.Serialize(Ar);
}

FLegacyMorphemeAnimRigToAnimMap* ULegacyMorphemeAnimSet::GetAnimRigToAnimMap()
{
	if (CurrentAnimRigToAnimMap == nullptr)
	{
		CurrentAnimRigToAnimMap = reinterpret_cast<FLegacyMorphemeAnimRigToAnimMap*>(CompiledRigToAnimMapDataPC64.BulkData+48);
		CurrentAnimRigToAnimMap->Unpack();
	}
	return CurrentAnimRigToAnimMap;
}

FLegacyMorphemeAnimRigDef* ULegacyMorphemeAnimSet::GetAnimRigDef()
{
	if (CurrentAnimRigDef == nullptr)
	{
		CurrentAnimRigDef = reinterpret_cast<FLegacyMorphemeAnimRigDef*>(CompiledRigDataPC64.BulkData+48);
		CurrentAnimRigDef->Unpack();
	}
	return CurrentAnimRigDef;
}
