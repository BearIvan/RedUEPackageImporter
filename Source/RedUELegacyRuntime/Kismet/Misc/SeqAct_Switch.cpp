#include "Kismet/Misc/SeqAct_Switch.h"

void USeqAct_Switch::In()
{
	for (int32 Idx = 0; Idx < Indices.Num(); Idx++)
	{
		int32 ActiveIdx = Indices[Idx] - 1;
		if (ActiveIdx >= 0 && ActiveIdx < CustomLinks.Num())
		{
			if (!DisabledIndices.Contains(ActiveIdx))
			{
				ExecuteCustomLink(ActiveIdx);
				if (bAutoDisableLinks)
				{
					DisabledIndices.Add(ActiveIdx);
				}
			}
		}
		// increment the indices
		if (IncrementAmount != 0)
		{
			if (bLooping)
			{
				Indices[Idx] = 1 + ((Indices[Idx] - 1 + IncrementAmount) %  CustomLinks.Num());
			}
			else
			{
				Indices[Idx] += IncrementAmount;
			}
		}
	}
}
