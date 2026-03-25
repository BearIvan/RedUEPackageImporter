#include "EngineSequenceActions.h"

#include "Blueprints/RedUEBlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

void USeqAct_SetMaterial::In()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		TArray<UPrimitiveComponent*> PrimitiveComponents;
		Actor->GetComponents(PrimitiveComponents);
		for (UPrimitiveComponent* PrimitiveComponent : PrimitiveComponents)
		{
			PrimitiveComponent->SetMaterial(MaterialIndex,NewMaterial);
		}
	}
	Out.Broadcast();
}

void USeqAct_SetObject::In()
{
	SetTargets(GetValue());
	Out.Broadcast();
}

void USeqAct_ToggleHidden::Hide()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		Actor->SetActorHiddenInGame(true);
	}
	Out.Broadcast();
}

void USeqAct_ToggleHidden::UnHide()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		Actor->SetActorHiddenInGame(false);
	}
	Out.Broadcast();
}

void USeqAct_ToggleHidden::Toggle()
{
	TArray<AActor*> InActors = GetActors();
	for (AActor* Actor : InActors)
	{
		if (!Actor)
		{
			continue;
		}
		Actor->SetActorHiddenInGame(!Actor->IsHidden());
	}
	Out.Broadcast();
}

void USeqAct_Toggle::TurnOn()
{
	Out.Broadcast();
}

void USeqAct_Toggle::TurnOff()
{
	Out.Broadcast();
}

void USeqAct_Toggle::Toggle()
{
	Out.Broadcast();
}

USeqAct_Delay::USeqAct_Delay()
{
	CurrentTime = 0;
	Paused = false;
}

void USeqAct_Delay::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentTime > 0 && !Paused)
	{
		CurrentTime -= DeltaTime;
		if (CurrentTime <= 0)
		{
			Finished.Broadcast();
		}
	}
}

void USeqAct_Delay::Start()
{
	if (Paused)
	{
		Paused = false;
		return;
	}
	CurrentTime = GetDuration();
}

void USeqAct_Delay::Stop()
{
	if (CurrentTime > 0)
	{
		Aborted.Broadcast();
	}
	Paused = false;
	CurrentTime = 0;
}

void USeqAct_Delay::Pause()
{
	if (CurrentTime > 0)
	{
		Paused = true;
	}
}

void UXSeqAct_PlayEffect::Play()
{
	Out.Broadcast();
}

void UXSeqAct_PlayEffect::Stop()
{
}

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

void USeqAct_SetPhysics::In()
{
	Out.Broadcast();
}

void USeqAct_Log::In()
{
	Out.Broadcast();
}

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

void USeqAct_Teleport::In()
{
	AActor* InDestination = GetDestination();
	if (InDestination)
	{
		TArray<AActor*> InTargets = GetTargets();
		for (AActor* Actor:InTargets)
		{
			if (APlayerController*PC = Cast<APlayerController>(Actor))
			{
				PC->GetPawn()->TeleportTo(InDestination->GetActorLocation(),InDestination->GetActorRotation());
			}
			else if (Actor)
			{
				Actor->TeleportTo(InDestination->GetActorLocation(),InDestination->GetActorRotation());
			}
		}
	}
	Out.Broadcast();
}

void USeqEvent_Console::In()
{
	Out.Broadcast();
}

void UXSeqAct_MultiLevelStreaming::BeginPlay()
{
	Super::BeginPlay();
}

void UXSeqAct_MultiLevelStreaming::Construct()
{
	Super::Construct();
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		Level.LevelStreaming = FStreamLevelAction::FindAndCacheLevelStreamingObject(Level.LevelName,GetWorld());
	}
}

void UXSeqAct_MultiLevelStreaming::Load()
{
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		if (ULevelStreaming* LevelStreaming = Level.LevelStreaming)
		{
			LevelStreaming->SetShouldBeLoaded(true);
			LevelStreaming->SetShouldBeVisible(true);
		}
	}
}

void UXSeqAct_MultiLevelStreaming::Unload()
{
	for (FLegacyLevelStreamingNameCombo&Level:Levels)
	{
		if (ULevelStreaming* LevelStreaming = Level.LevelStreaming)
		{
			LevelStreaming->SetShouldBeLoaded(false);
		}
	}
}

void USeqAct_WaitForLevelsVisible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!Levels.IsEmpty())
	{
		for (ULevelStreaming* LevelStreaming : Levels)
		{
			if (LevelStreaming->IsLevelVisible())
			{
				Levels.Remove(LevelStreaming);
			}
		}
	
		if (Levels.IsEmpty())
		{
			Finished.Broadcast();
		}
	}
	
}

void USeqAct_WaitForLevelsVisible::Wait()
{
	Levels.Empty();
	
	class UWorld* InWorld = GetWorld();
	for (FName LevelName:LevelNames)
	{
		if (ULevelStreaming* LocalLevel = FStreamLevelAction::FindAndCacheLevelStreamingObject( LevelName, InWorld ))
		{
			Levels.Add(LocalLevel);
			LocalLevel->bShouldBlockOnLoad = bShouldBlockOnLoad;
		}
	}
	if (Levels.IsEmpty())
	{
		Finished.Broadcast();
	}
}

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
