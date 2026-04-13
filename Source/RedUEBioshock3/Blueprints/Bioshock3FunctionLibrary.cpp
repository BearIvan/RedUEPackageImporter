#include "Bioshock3FunctionLibrary.h"

#include "Core/PlayerState/XPlayerStateComponent.h"
#include "GameFramework/PlayerState.h"

UXPlayerStateComponent* UBioshock3FunctionLibrary::GetXPlayerStateComponent(APlayerController* Player)
{
	if (Player)
	{
		if (APlayerState* PS = Player->PlayerState)
		{
			if (UXPlayerStateComponent* Result = PS->FindComponentByClass<UXPlayerStateComponent>())
			{
				return Result;
			}
			return CastChecked<UXPlayerStateComponent>(PS->AddComponentByClass(UXPlayerStateComponent::StaticClass(),false,FTransform::Identity,false),ECastCheckedType::NullAllowed);
		}
	}
	return nullptr;
}
