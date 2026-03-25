#include "XSeqAct_PlaySound.h"
#include "Kismet/GameplayStatics.h"

void UXSeqAct_PlaySound::Play()
{
	TArray<AActor*> InActors = GetActors();
	if (InActors.Num()  == 0)
	{
		UGameplayStatics::SpawnSound2D(GetWorld(),PlaySound);
	}
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		UGameplayStatics::SpawnSoundAttached(PlaySound,Actor->GetRootComponent());
	}
	Out.Broadcast();
}

void UXSeqAct_PlaySound::Stop()
{
	Stopped.Broadcast();
}
