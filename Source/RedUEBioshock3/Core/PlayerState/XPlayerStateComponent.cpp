#include "XPlayerStateComponent.h"


UXPlayerStateComponent::UXPlayerStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UXPlayerStateComponent::DPVAddCount(FName Name, int32 Count)
{
	if (int32*DPV = DPVCounters.Find(Name))
	{
		*DPV += Count;
	}
	else
	{
		DPVCounters.Add(Name, Count);
	}
}

int32 UXPlayerStateComponent::DPVGetCount(FName Name) const
{
	if (const int32* DPV = DPVCounters.Find(Name))
	{
		return *DPV;
	}
	return 0;
}


void UXPlayerStateComponent::DPVResetCount(FName Name)
{
	DPVCounters.Remove(Name);
}


