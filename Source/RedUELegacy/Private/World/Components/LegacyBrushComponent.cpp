// Fill out your copyright notice in the Description page of Project Settings.


#include "World/Components/LegacyBrushComponent.h"

#include "Mesh/LegacyModel.h"

void ULegacyBrushComponent::FillComponent_Implementation(UActorComponent* InActorComponent)
{
	Super::FillComponent_Implementation(InActorComponent);
	if (UBrushComponent* BrushComponent = Cast<UBrushComponent>(InActorComponent))
	{
		ABrush* BrushActor = BrushComponent->GetOwner<ABrush>();
		Brush->Fill(BrushActor);
	}
}
