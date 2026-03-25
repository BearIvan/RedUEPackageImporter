#pragma once

#include "XFloatingSection.generated.h"

UCLASS()
class REDUEBIOSHOCK3_API AXFloatingSection : public AActor
{
	GENERATED_BODY()

public:
					AXFloatingSection	();
	virtual void	Tick				(float DeltaTime) override;

	
protected:
	virtual void	BeginPlay			() override;
	virtual void	EndPlay				(const EEndPlayReason::Type EndPlayReason) override;
private:
			void	LevelAddedToWorld	(ULevel* Level, UWorld* World);

	FDelegateHandle OnLevelAddedToWorld;
protected:

public:
	UPROPERTY(EditAnywhere,Category="Legacy")
	TArray<FName> LevelNames;

	UPROPERTY(VisibleAnywhere,Category="Legacy")
	USceneComponent *SceneComponent;
	
	UPROPERTY(EditAnywhere,Category="Legacy")
	FMatrix InvInitialTransform;

#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere,Category="Legacy")
	UBillboardComponent *Billboard;
#endif
private:
	
	UPROPERTY(Transient,VisibleAnywhere,Category="Legacy")
	TArray<bool> IsLoadedLevels;
	
	UPROPERTY(Transient,VisibleAnywhere,Category="Legacy")
	TArray<ULevelStreaming*> Levels;
	
	
	UPROPERTY(Transient)
	FTransform InitialTransform;
};
