#include "XFloatingSection.h"

#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

#include "Components/BillboardComponent.h"


AXFloatingSection::AXFloatingSection()
{
	PrimaryActorTick.bCanEverTick = false;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);
#if WITH_EDITORONLY_DATA
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetupAttachment(SceneComponent);
#endif
}

void AXFloatingSection::BeginPlay()
{
	Super::BeginPlay();
	
	for (FName LevelName:LevelNames)
	{
		ULevelStreaming*LevelStreaming = UGameplayStatics::GetStreamingLevel(GetWorld(),LevelName);
		if (LevelStreaming)
		{
			Levels.Add(LevelStreaming);
			IsLoadedLevels.Add(!LevelStreaming->IsLevelLoaded());
		}
	}
	
	LevelAddedToWorld(nullptr,nullptr);
}

void AXFloatingSection::LevelAddedToWorld(ULevel* Level, UWorld* World)
{
	for (int32  i = 0; i < Levels.Num(); i++)
	{
		ULevelStreaming*LevelStreaming = Levels[i];
		if (LevelStreaming[i].IsLevelLoaded() != IsLoadedLevels[i])
		{
			if (LevelStreaming[i].IsLevelLoaded())
			{
				for (AActor* Actor : LevelStreaming->GetLoadedLevel()->Actors)
				{
					Actor->AttachToActor(this,FAttachmentTransformRules::KeepWorldTransform);
					// 
					// Actor->SetActorTransform( NewTransform);
				}
			}
		}
		IsLoadedLevels[i] = LevelStreaming[i].IsLevelLoaded();
	}
}

void AXFloatingSection::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

