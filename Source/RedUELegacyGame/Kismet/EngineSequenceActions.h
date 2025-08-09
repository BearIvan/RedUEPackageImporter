#pragma once
#include "Base/SequenceAction.h"
#include "EngineSequenceActions.generated.h"

UCLASS()
class REDUELEGACYGAME_API USeqAct_SetMaterial : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;
	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Actors);
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* NewMaterial;
	
	UPROPERTY(EditAnywhere)
	int32 MaterialIndex = 0;
};


UCLASS()
class REDUELEGACYGAME_API USeqAct_ToggleHidden : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Hide();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void UnHide();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Actors);
};


UCLASS()
class REDUELEGACYGAME_API USeqAct_Toggle : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void TurnOn();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void TurnOff();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(meta = (KismetEvent,LegacyIndex = 0))
	TArray<FGuid> Event;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Actors);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	bool Bool;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(bool,Bool);
	
};

UCLASS()
class REDUELEGACYGAME_API USeqAct_Delay : public USequenceAction
{
	GENERATED_BODY()
public:
					USeqAct_Delay		();
	virtual void	Tick				(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Start();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Stop();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Pause();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Aborted;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(KismetExternalVariable ,LegacyIndex = 0))
	float Duration = 1;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(float,Duration);
private:
	UPROPERTY()
	float CurrentTime;

	UPROPERTY()
	bool Paused;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_PlayEffect: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Play();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Stop();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate Stopped;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Actors);
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_PlaySound: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Play();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Stop();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate Stopped;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Actors);
	
	UPROPERTY(EditAnywhere)
	USoundBase* PlaySound;
	
	UPROPERTY(EditAnywhere)
	USoundBase* StopSound;
	
};


UCLASS()
class REDUELEGACYGAME_API USeqAct_SetPhysics: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Actors;

	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Actors);
};

UCLASS()
class REDUELEGACYGAME_API USeqAct_Log: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};



UCLASS()
class REDUELEGACYGAME_API USeqAct_CameraFade: public USequenceAction
{
	GENERATED_BODY()
public:
					USeqAct_CameraFade	();
	virtual void	Tick				(float DeltaTime) override;
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate Finished;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Targets)
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FadeOpacity;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FadeTime;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float BeginFadeOpacity;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FColor FadeColor;
	
private:

	UPROPERTY()
	APlayerController* CachePC;
};

UCLASS()
class REDUELEGACYGAME_API USeqAct_Teleport: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	TArray<AActor*> Targets;
	SEQUENCE_ACTION_KISMET_ARRAY_ATTRIBUTE(AActor*,Targets)

	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	AActor* Destination;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(AActor*,Destination)
	
};

UCLASS()
class REDUELEGACYGAME_API USeqEvent_Console: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ConsoleEventName;
};

USTRUCT(Blueprintable)
struct FLegacyLevelStreamingNameCombo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName LevelName;
};
UCLASS()
class REDUELEGACYGAME_API UXSeqAct_MultiLevelStreaming: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Load();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Unload();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FLegacyLevelStreamingNameCombo> Levels;
};



