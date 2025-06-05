#include "LegacyKismet.h"

#include "LegacyKismetGeneratedClass.h"
#include "SequenceAction.h"

class ULegacyKismetGeneratedClass;

ALegacyKismet::ALegacyKismet()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALegacyKismet::BeginPlay()
{
	if (ULegacyKismetGeneratedClass* KismetGeneratedClass = Cast<ULegacyKismetGeneratedClass>(GetClass()))
	{
		for (auto & [Key,Value] :KismetGeneratedClass->StartupActions)
		{
			USequenceAction* NewAction = SequenceActions.Add(Key,DuplicateObject(Value,this));
			NewAction->Construct();
		
		}
	}
	Super::BeginPlay();
	for (auto & [Key,Value] :SequenceActions)
	{
		Value->BeginPlay();
	}
}

void ALegacyKismet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	for (auto & [Key,Value] :SequenceActions)
	{
		Value->Tick(DeltaTime);
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
