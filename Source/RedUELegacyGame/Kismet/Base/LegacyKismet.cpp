#include "LegacyKismet.h"

#include "EngineUtils.h"
#include "LegacyKismetGeneratedClass.h"
#include "SequenceAction.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/Event/SeqEvent_RemoteEvent.h"

class ULegacyKismetGeneratedClass;

ALegacyKismet::ALegacyKismet()
{
	PrimaryActorTick.bCanEverTick = true;
	bFirstTick = false;
}

void ALegacyKismet::BeginPlay()
{
	if (ULegacyKismetGeneratedClass* KismetGeneratedClass = Cast<ULegacyKismetGeneratedClass>(GetClass()))
	{
		for (auto & [Key,Value] :KismetGeneratedClass->StartupActions)
		{
			USequenceAction* NewAction = SequenceActions.Add(Key,DuplicateObject(Value,this));
			NewAction->Construct();
			if (USeqEvent_RemoteEvent* RemoveEvent = Cast<USeqEvent_RemoteEvent>(NewAction))
			{
				SequenceRemoteEvents.Add(RemoveEvent->EventName,RemoveEvent);
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
	Super::Tick(DeltaTime);
	for (auto & [Key,Value] :SequenceActions)
	{
		Value->Tick(DeltaTime);
	}
}

void ALegacyKismet::ActivateRemoteEvent(const FName& InName)
{
	if (USeqEvent_RemoteEvent** RemoteEvent = SequenceRemoteEvents.Find(InName))
	{
		if ((*RemoteEvent)->bEnabled)
		{
			(*RemoteEvent)->Out.Broadcast();
		}
	}
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

