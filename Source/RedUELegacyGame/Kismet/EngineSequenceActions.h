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
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
	
	UPROPERTY(EditAnywhere)
	UMaterialInterface* NewMaterial;
	
	UPROPERTY(EditAnywhere)
	int32 MaterialIndex = 0;
};


UCLASS()
class REDUELEGACYGAME_API USeqAct_SetObject: public USequenceAction
{
	GENERATED_BODY()
public:
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0 ))
	AActor* Value;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Value);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	TArray<AActor*> Targets;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets);
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	bool Bool;

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Bool);
	
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Duration);
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
	
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Actors);
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

	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets)
	
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
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Targets)

	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1 ))
	AActor* Destination;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Destination)
	
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
	
	UPROPERTY(Transient)
	ULevelStreaming* LevelStreaming;
};
UCLASS()
class REDUELEGACYGAME_API UXSeqAct_MultiLevelStreaming: public USequenceAction
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void Construct() override;
	
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Load();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Unload();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FLegacyLevelStreamingNameCombo> Levels;
	
	
};

UCLASS()
class REDUELEGACYGAME_API USeqAct_WaitForLevelsVisible: public USequenceAction
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Wait();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Finished;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FName> LevelNames;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShouldBlockOnLoad;
	
	UPROPERTY(Transient)
	TArray<ULevelStreaming*> Levels;
};



UCLASS()
class REDUELEGACYGAME_API USeqAct_GetDistance: public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 0))
	AActor* A;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(A);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	AActor* B;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(B);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 2))
	float Distance;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(Distance);
	
};



