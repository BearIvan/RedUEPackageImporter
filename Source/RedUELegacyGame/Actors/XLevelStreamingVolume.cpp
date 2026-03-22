#include "XLevelStreamingVolume.h"
#include "Components/BrushComponent.h"


AXLevelStreamingVolume::AXLevelStreamingVolume()
{
	static FName CollisionProfileName(TEXT("OverlapOnlyPawn"));
	GetBrushComponent()->SetCollisionProfileName(CollisionProfileName);
	PrimaryActorTick.bCanEverTick = false;
}

void AXLevelStreamingVolume::BeginPlay()
{
	Super::BeginPlay();
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		NotifyActorBeginOverlap(Actor);
	}
	GetWorld()->OnWorldMatchStarting.AddUObject(this,&AXLevelStreamingVolume::OnWorldMatchStarting);
}


void AXLevelStreamingVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	if (!HasActorBegunPlay())
	{
		return;
	}
	
	if (!bLevelStreamingActive)
	{
		return;
	}
	
	if (APawn* OtherPawn = Cast<APawn>(OtherActor))
	{
		if (OtherPawn->IsPlayerControlled())
		{
			UWorld* InWorld = GetWorld();
			if (CachedLevelsStreaming.Num() != Levels.Num())
			{
				CachedLevelsStreaming.Empty();
				for (const FXLevelAndStreamingStatus& Level : Levels)
				{
					CachedLevelsStreaming.Add(FStreamLevelAction::FindAndCacheLevelStreamingObject(Level.LevelName,InWorld));
				}
			}
			for (int32  i = 0; i < CachedLevelsStreaming.Num(); i++)
			{
				if (CachedLevelsStreaming[i])
				{
					switch (Levels[i].DesiredStatus)
					{
					case SVLS_Visible: CachedLevelsStreaming[i]->SetShouldBeLoaded(true); CachedLevelsStreaming[i]->SetShouldBeVisible(true); break;
					case SVLS_Loaded: CachedLevelsStreaming[i]->SetShouldBeLoaded(true);  CachedLevelsStreaming[i]->SetShouldBeVisible(false); break;
					default: ;
					}	
				}
			}
		}
	}
}

void AXLevelStreamingVolume::OnWorldMatchStarting()
{
	GetWorld()->OnWorldMatchStarting.RemoveAll(this);
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors);
	
	for (AActor* Actor : OverlappingActors)
	{
		NotifyActorBeginOverlap(Actor);
	}
}

