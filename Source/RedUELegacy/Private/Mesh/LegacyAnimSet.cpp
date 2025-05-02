#include "Mesh/LegacyAnimSet.h"

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
