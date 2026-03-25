#include "Kismet/Engine/SeqAct_CameraFade.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"

USeqAct_CameraFade::USeqAct_CameraFade()
{
	FadeOpacity = 1;
	FadeTime = 1;
	FadeColor = FColor::Black;
	BeginFadeOpacity = 0;
}

void USeqAct_CameraFade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CachePC)
	{
		if (!CachePC->PlayerCameraManager->bEnableFading)
		{
			CachePC = nullptr;
			Finished.Broadcast();
		}
	}
}

void USeqAct_CameraFade::In()
{
	CachePC = nullptr;
	TArray<AActor*> InTargets = GetTargets();
	if (Targets.Num() > 0)
	{
		CachePC = Cast<APlayerController>(Targets[0]);
	}
	if (!CachePC)
	{
		CachePC = UGameplayStatics::GetPlayerController(this,0);
	}
	if (CachePC)
	{
		CachePC->PlayerCameraManager->StartCameraFade(BeginFadeOpacity,FadeOpacity,FadeTime,FadeColor);
	}
	Out.Broadcast();
}
