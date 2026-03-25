#pragma once

#include "Sequence/LegacySequenceObjects.h"
#include "UObject/Object.h"
#include "LegacyXSequence.generated.h"





UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXSeqVar_PlayerController : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXSeqVar_Elizabeth : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	
};
UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXSeqVar_SinglePlayerController : public ULegacyXSeqVar_PlayerController
{
	GENERATED_BODY()
};


UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternCommand_Base: public ULegacyObject
{
	GENERATED_BODY()
public:
	
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern);
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternUObjectSetBase: public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual class UXPatternObjectSetBase* MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern);
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternUObjectSet_PlayerPawns: public ULegacyXPatternUObjectSetBase
{
	GENERATED_BODY()
public:
	virtual class UXPatternObjectSetBase* MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern) override;
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternUObjectSet_ElizabethPawns: public ULegacyXPatternUObjectSetBase
{
	GENERATED_BODY()
public:
	virtual class UXPatternObjectSetBase* MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern) override;
};
UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternCommand_ActivateNamedPatternKismetEvent: public ULegacyXPatternCommand_Base
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
	
	UPROPERTY(BlueprintReadWrite)
	FName KismetPatternEventName;
};


UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternCommand_PlaySound: public ULegacyXPatternCommand_Base
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyXEffectSound* PlaySound;

	UPROPERTY(BlueprintReadWrite)
	ULegacyXPatternUObjectSetBase* Targets;
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternCommand_PlaySpeech: public ULegacyXPatternCommand_Base
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyXEffectSpeechPostEvent* SpeechRequest;

	UPROPERTY(BlueprintReadWrite)
	ULegacyXPatternUObjectSetBase* PossibleSpeakers;
};
UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternEvent_TimeElapsed: public ULegacyXPatternCommand_Base
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
	
	UPROPERTY(BlueprintReadWrite)
	float SleepDurationSeconds = 0.f;
	
	UPROPERTY(BlueprintReadWrite)
	bool bUseAudioTime = false;
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPatternEvent_AudioTimeElapsed: public ULegacyXPatternEvent_TimeElapsed
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
};

UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXPattern_AutomaticallyRun : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual void ExportToInstancePattern(class UXSeqAct_InstancePattern* InstancePattern);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyXPatternCommand_Base*> Sequence;
};


UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXSeqAct_InstancePattern : public ULegacySequenceImporter
{
	GENERATED_BODY()
public:
	ULegacyXSeqAct_InstancePattern();

	virtual UK2Node* 					ExportToBlueprint	(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						FillAction			(USequenceAction* InSequenceAction) override;

	UPROPERTY(BlueprintReadWrite)
	ULegacyXPattern_AutomaticallyRun*	BasePatternArchetype;

	UPROPERTY()
	TMap<FName,FName> EventNameToFullName;
};


UCLASS()
class REDUEBIOSHOCK3IMPORTER_API ULegacyXSeqAct_PlaySound : public ULegacySequenceImporter
{
	GENERATED_BODY()
public:
	ULegacyXSeqAct_PlaySound	();
	virtual void					FillAction					(USequenceAction* InSequenceAction) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyXEffectSound* PlaySound;
};


USTRUCT(Blueprintable)
struct FLegacyXAKEventTrackKey
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	float Time = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyXAKAudioEventID* Event;

};

UCLASS()
class ULegacyXAKInterpTrackPostEvent : public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyXAKEventTrackKey> AKEvents;
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
};
