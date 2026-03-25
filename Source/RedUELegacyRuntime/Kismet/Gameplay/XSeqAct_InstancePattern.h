#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "XSeqAct_InstancePattern.generated.h"


UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACYRUNTIME_API UXPatternCommandBase:public UObject
{
	GENERATED_BODY()
public:
					UXPatternCommandBase();
	virtual void 	Begin				();
	virtual bool 	CanJumpNextCommand	();
	virtual void 	Tick				(float DeltaTime);
	virtual void 	End					();
};

UCLASS(EditInlineNew,DefaultToInstanced)
class REDUELEGACYRUNTIME_API UXPatternObjectSetBase:public UObject
{
	GENERATED_BODY()
public:
					UXPatternObjectSetBase	();
	virtual APawn* 	GetPawn					();
};

UCLASS()
class REDUELEGACYRUNTIME_API UXPatternObjectSet_Player:public UXPatternObjectSetBase
{
	GENERATED_BODY()
public:
	UXPatternObjectSet_Player				();
	virtual APawn* 	GetPawn					() override;
};


UCLASS()
class REDUELEGACYRUNTIME_API UXPatternObjectSet_ElizabethPawn:public UXPatternObjectSetBase
{
	GENERATED_BODY()
public:
	UXPatternObjectSet_ElizabethPawn	();
	virtual APawn* 	GetPawn				() override;
};

UCLASS()
class REDUELEGACYRUNTIME_API UXPatternCommand_KismetEvent:public UXPatternCommandBase
{
	GENERATED_BODY()
public:
					UXPatternCommand_KismetEvent	();
	virtual void 	Begin							() override;
	UPROPERTY(EditAnywhere)
	FName EventName;
};


UCLASS()
class REDUELEGACYRUNTIME_API UXPatternCommand_Speak:public UXPatternCommandBase
{
	GENERATED_BODY()
public:
					UXPatternCommand_Speak	();
	virtual void 	Begin					() override;
	
	UPROPERTY(EditAnywhere)
	USoundBase* Audio;

	UPROPERTY(EditAnywhere)
	UXPatternObjectSetBase* Speaker = nullptr;
};

UCLASS()
class REDUELEGACYRUNTIME_API UXPatternCommand_PlayAudio:public UXPatternCommandBase
{
	GENERATED_BODY()
public:
					UXPatternCommand_PlayAudio	();
	virtual void 	Begin						() override;
	
	UPROPERTY(EditAnywhere)
	USoundBase* Audio;

	UPROPERTY(EditAnywhere)
	UXPatternObjectSetBase* Target = nullptr;
};



UCLASS()
class REDUELEGACYRUNTIME_API UXPatternCommand_TimeElapsed:public UXPatternCommandBase
{
	GENERATED_BODY()
public:
					UXPatternCommand_TimeElapsed		();
	virtual void 	Begin								() override;
	virtual void 	Tick								(float Time) override;
	virtual bool 	CanJumpNextCommand					() override;
	
	UPROPERTY(EditAnywhere)
	float SleepDurationSeconds = 0;
	
	UPROPERTY(EditAnywhere)
	bool bUseAudioTime = false;

	UPROPERTY()
	float CurrentTime = 0;

};


UCLASS()
class REDUELEGACYRUNTIME_API UXPatternCommand_AudioTimeElapsed:public UXPatternCommand_TimeElapsed
{
	GENERATED_BODY()
public:
	UXPatternCommand_AudioTimeElapsed	();
	virtual void 	Begin								() override;
	virtual void 	Tick								(float Time) override;
	virtual bool 	CanJumpNextCommand					() override;
};


UCLASS()
class REDUELEGACYRUNTIME_API UXSeqAct_InstancePattern : public USequenceAction
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void DestroyInstance();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate PatternInstanced;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 2))
	FSequenceActionDelegate PatternFailed;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 3))
	FSequenceActionDelegate PatternMatched;

	UPROPERTY()
	APawn*LastPawn = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<UXPatternCommandBase*> Commands;

	UPROPERTY(VisibleAnywhere)
	int32 CurrentCommands = INDEX_NONE;

	UPROPERTY(VisibleAnywhere)
	UAudioComponent* LastAudio = nullptr;
	
	
};
