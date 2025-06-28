#include "SeqAct_ActivateRemoteEvent.h"

#include "EngineUtils.h"
#include "Kismet/Base/LegacyKismet.h"

void USeqAct_ActivateRemoteEvent::Broadcast()
{
	for (TActorIterator<ALegacyKismet> ActorItr(GetOwnerKismetChecked()->GetWorld()); ActorItr; ++ActorItr)
	{
		ActorItr->ActivateRemoteEvent(EventName);
	}
	Out.Broadcast();
}
