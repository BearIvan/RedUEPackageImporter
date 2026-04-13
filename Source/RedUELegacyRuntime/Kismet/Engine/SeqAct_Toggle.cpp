#include "Kismet/Engine/SeqAct_Toggle.h"

#include "Kismet/Base/LegacyKismet.h"

void USeqAct_Toggle::TurnOn()
{
	ALegacyKismet* OwnerKismet = GetOwnerKismetChecked();
	for (FGuid EventGuid:Event)
	{
		if (USequenceAction** SequenceAction = OwnerKismet->SequenceActions.Find(EventGuid))
		{
			if (USequenceEvent* SeqEvent = Cast<USequenceEvent>(*SequenceAction))
			{
				SeqEvent->bEnabled = true;
			}
		}
	}
	
	SetBooleans(true);
	Out.Broadcast();
}

void USeqAct_Toggle::TurnOff()
{
	ALegacyKismet* OwnerKismet = GetOwnerKismetChecked();
	for (FGuid EventGuid:Event)
	{
		if (USequenceAction** SequenceAction = OwnerKismet->SequenceActions.Find(EventGuid))
		{
			if (USequenceEvent* SeqEvent = Cast<USequenceEvent>(*SequenceAction))
			{
				SeqEvent->bEnabled = false;
			}
		}
	}
	SetBooleans(false);
	Out.Broadcast();
}

void USeqAct_Toggle::Toggle()
{
	ALegacyKismet* OwnerKismet = GetOwnerKismetChecked();
	for (FGuid EventGuid:Event)
	{
		if (USequenceAction** SequenceAction = OwnerKismet->SequenceActions.Find(EventGuid))
		{
			if (USequenceEvent* SeqEvent = Cast<USequenceEvent>(*SequenceAction))
			{
				SeqEvent->bEnabled = !SeqEvent->bEnabled;
			}
		}
	}
	TArray<bool> InOutBooleans = GetBooleans();
	for (bool& InOutBoolean:InOutBooleans)
	{
		InOutBoolean = !InOutBoolean;
	}
	SetBooleans(InOutBooleans);
	Out.Broadcast();
}
