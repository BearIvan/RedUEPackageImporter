#include "Kismet/Engine/SeqAct_GetDistance.h"
#include "GameFramework/Controller.h"

void USeqAct_GetDistance::In()
{
	float InDistance = UE_MAX_FLT;
	AActor* InA = GetA();
	AActor* InB = GetB();
	if (AController* AAsController = Cast<AController>(InA))
	{
		InA = AAsController->GetPawn();
	}
	if (AController* BAsController = Cast<AController>(InB))
	{
		InB = BAsController->GetPawn();
	}
	if (InA && InB)
	{
		InDistance = FVector::Dist(InA->GetActorLocation(),InB->GetActorLocation());
	}
	SetDistance(InDistance);
	Out.Broadcast();
}
