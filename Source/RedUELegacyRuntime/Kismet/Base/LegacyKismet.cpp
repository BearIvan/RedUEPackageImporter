#include "LegacyKismet.h"

#include "EngineUtils.h"
#include "SequenceAction.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/Event/SeqEvent_RemoteEvent.h"

ALegacyKismet::ALegacyKismet()
{
	PrimaryActorTick.bCanEverTick = true;
	bFirstTick = false;
}

void ALegacyKismet::BeginPlay()
{
	// if (const ALegacyKismet* LegacyKismetCDO = GetClass()->GetDefaultObject<ALegacyKismet>())
	// {
	// 	for (const auto& [Key, Value] : LegacyKismetCDO->StartupActions)
	// 	{
	// 		USequenceAction* NewAction = SequenceActions.Add(Key, DuplicateObject(Value, this));
	// 		NewAction->Construct();
	// 		if (USeqEvent_RemoteEvent* RemoveEvent = Cast<USeqEvent_RemoteEvent>(NewAction))
	// 		{
	// 			if (USeqEvent_RemoteEvent** RemoteEvent = SequenceRemoteEvents.Find(RemoveEvent->EventName))
	// 			{
	// 				USeqEvent_RemoteEvent* CurrentEvent = *RemoteEvent;
	// 				for (;CurrentEvent->NextEvent;CurrentEvent = CurrentEvent->NextEvent){}
	// 				CurrentEvent->NextEvent = RemoveEvent;
	// 			}
	// 			else
	// 			{
	// 				SequenceRemoteEvents.Add(RemoveEvent->EventName, RemoveEvent);
	// 			}
	// 		}
	// 	}
	// }
	
	SequenceRemoteEvents.Empty();
	for (auto& [Key, Value] : SequenceActions)
	{
		Value->Construct();
		if (USeqEvent_RemoteEvent* RemoveEvent = Cast<USeqEvent_RemoteEvent>(Value))
		{
			if (USeqEvent_RemoteEvent** RemoteEvent = SequenceRemoteEvents.Find(RemoveEvent->EventName))
			{
				USeqEvent_RemoteEvent* CurrentEvent = *RemoteEvent;
				for (;CurrentEvent->NextEvent;CurrentEvent = CurrentEvent->NextEvent){}
				CurrentEvent->NextEvent = RemoveEvent;
			}
			else
			{
				SequenceRemoteEvents.Add(RemoveEvent->EventName, RemoveEvent);
			}
		}
	}
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	Super::BeginPlay();
}

void ALegacyKismet::Tick(float DeltaTime)
{
	if (!bFirstTick)
	{
		for (auto & [Key,Value] :SequenceActions)
		{
			Value->BeginPlay();
		}
		bFirstTick = true;
	}
	else
	{
		TArray<FName> InRemoteEventNames;
		Swap(RemoteEventNames,InRemoteEventNames);
		for (const FName& InName : InRemoteEventNames)
		{
			if (USeqEvent_RemoteEvent** RemoteEvent = SequenceRemoteEvents.Find(InName))
			{
				USeqEvent_RemoteEvent* CurrentEvent = *RemoteEvent;
				for (;CurrentEvent;CurrentEvent = CurrentEvent->NextEvent)
				{
					if (CurrentEvent->bEnabled)
					{
						CurrentEvent->Out.Broadcast();
					}
				}
			}
		}
		
		Super::Tick(DeltaTime);
		for (auto & [Key,Value] :SequenceActions)
		{
			Value->Tick(DeltaTime);
		}
	}
	


}

void ALegacyKismet::ActivateRemoteEvent(const FName& InName)
{
	RemoteEventNames.AddUnique(InName);
}

USequenceAction* ALegacyKismet::GetSequenceAction(FGuid ActionGuid, TSubclassOf<USequenceAction> SequenceActionClass)
{
	if (USequenceAction** Action = SequenceActions.Find(ActionGuid))
	{
		if (*Action&&(*Action)->GetClass()->IsChildOf(SequenceActionClass))
		{
			return *Action;
		}
	}
	return nullptr;
}

ABioshockKismet::ABioshockKismet(): ElizabethPawn(nullptr)
{
}

void ABioshockKismet::RefreshElizabethPawn()
{
	static FName NAME_Elizabeth = "Elizabeth";
	if (ElizabethPawn == nullptr)
	{
		for (TActorIterator<APawn> It(GetWorld(), APawn::StaticClass()); It; ++It)
		{
			APawn* Actor = *It;
			if (IsValid(Actor) && Actor->ActorHasTag(NAME_Elizabeth))
			{
				ElizabethPawn = Actor;
				break;
			}
		}
	}
}

