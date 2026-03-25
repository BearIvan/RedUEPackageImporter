#pragma once

#include "LevelSequence.h"
#include "MovieScene.h"
#include "Core/LegacyObject.h"
#include "LegacySequenceObjects.generated.h"
class ALegacyKismet;
class UK2Node_SequenceAction;
class UK2Node;
class ULevelSequence;
class ISequencer;
class ULegacyActor;

USTRUCT(Blueprintable)
struct FLegacySeqOpInputLink
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString LinkDesc;
	
	UPROPERTY(BlueprintReadWrite)
	FName XLinkName;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasImpulse;
	
	UPROPERTY(BlueprintReadWrite)
	int32 QueuedActivations;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisabled;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisabledPIE;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacySequenceOp* LinkedOp  = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHidden;
	
	UPROPERTY(BlueprintReadWrite)
	float ActivateDelay;
	
	UPROPERTY(BlueprintReadWrite)
	bool bMoving;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMax;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMin;
	
	UPROPERTY(BlueprintReadWrite)
	int32 OverrideDelta;
};

USTRUCT(Blueprintable)
struct FLegacySeqOpOutputInputLink
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacySequenceOp* LinkedOp  = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	int32 InputLinkIdx;
};

USTRUCT(Blueprintable)
struct FLegacySeqOpOutputLink
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacySeqOpOutputInputLink> Links;
	
	UPROPERTY(BlueprintReadWrite)
	FString LinkDesc;
	
	UPROPERTY(BlueprintReadWrite)
	FName XLinkName;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHasImpulse;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisabled;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisabledPIE;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacySequenceOp* LinkedOp = nullptr;
	
	UPROPERTY(BlueprintReadWrite)
	float ActivateDelay;
	
	UPROPERTY(BlueprintReadWrite)
	int32 DrawY;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHidden;
	
	UPROPERTY(BlueprintReadWrite)
	bool bMoving;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMax;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMin;
	
	UPROPERTY(BlueprintReadWrite)
	bool bIsActivated = false;
};

USTRUCT(Blueprintable)
struct FLegacySeqVarLink
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	class UClass* ExpectedType;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceVariable*> LinkedVariables;
	
	UPROPERTY(BlueprintReadWrite)
	FString LinkDesc;
	
	UPROPERTY(BlueprintReadWrite)
	FName XLinkName;
	
	UPROPERTY(BlueprintReadWrite)
	FName LinkVar;
	
	UPROPERTY(BlueprintReadWrite)
	FName PropertyName;
	
	UPROPERTY(BlueprintReadWrite)
	bool bWriteable;
	
	UPROPERTY(BlueprintReadWrite)
	bool bSequenceNeverReadsOnlyWritesToThisVar;
	
	UPROPERTY(BlueprintReadWrite)
	bool bModifiesLinkedObject;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHidden;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MinVars;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxVars;
	
	UPROPERTY(BlueprintReadWrite)
	int32 DrawX;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyProperty* CachedProperty;
	
	UPROPERTY(BlueprintReadWrite)
	bool bAllowAnyType;
	
	UPROPERTY(BlueprintReadWrite)
	bool bMoving;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMax;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMin;
};

USTRUCT(Blueprintable)
struct FLegacySeqEventLink
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceOp*> LinkedEvents;
	
	UPROPERTY(BlueprintReadWrite)
	FString LinkDesc;
	
	UPROPERTY(BlueprintReadWrite)
	int32 DrawX;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHidden;
	
	UPROPERTY(BlueprintReadWrite)
	bool bMoving;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMax;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClampedMin;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySequenceObject : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual UK2Node* ExportToBlueprint	(UBlueprint* InBlueprint,UEdGraph* InGraph);
	
protected:
	UPROPERTY(Transient)
	UK2Node* CurrentNode = nullptr;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySequenceOp : public ULegacySequenceObject
{
	GENERATED_BODY()
public:
	virtual UEdGraphPin*			GetInputPin			(int32 Index,UBlueprint* InBlueprint,UEdGraph* InGraph);
	virtual UEdGraphPin*			GetEventPin			(UBlueprint* InBlueprint,UEdGraph* InGraph);
	virtual void					SimulatedImport		();
	static	void					FillPin				(UBlueprint* InBlueprint, UEdGraph* InGraph, const FLegacySeqOpOutputLink& LegacySeqOpOutputLink, UEdGraphPin* OutputPin);
	
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacySeqOpInputLink> InputLinks;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacySeqOpOutputLink> OutputLinks;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacySeqVarLink> VariableLinks;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacySeqEventLink> EventLinks;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySequenceImporter : public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	virtual void					FillAction						(class USequenceAction* InSequenceAction);
	virtual UK2Node*				ExportToBlueprint				(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void					PreLegacySerializeUnrealProps	(FRedUELegacyArchive& Ar) override;

	UPROPERTY(Transient)
	class USequenceAction* ToAction = nullptr;


	bool bNeedSerializeToActon = true;
};



UCLASS()
class REDUELEGACYIMPORTER_API ULegacySequenceAction : public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	FName HandlerName;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCallHandler;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class UObject*> Targets;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacySequenceEvent: public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacySequenceEvent*> DuplicateEvts;
	
	UPROPERTY(BlueprintReadWrite)
	class AActor* Originator;
	
	UPROPERTY(BlueprintReadWrite)
	class AActor* Instigator;
	
	UPROPERTY(BlueprintReadWrite)
	float ActivationTime;
	
	UPROPERTY(BlueprintReadWrite)
	int32 TriggerCount;
	
	UPROPERTY(BlueprintReadWrite)
	int32 MaxTriggerCount;
	
	UPROPERTY(BlueprintReadWrite)
	float ReTriggerDelay;
	
	UPROPERTY(BlueprintReadWrite)
	bool bEnabled;
	
	UPROPERTY(BlueprintReadWrite)
	bool bPlayerOnly;
	
	UPROPERTY(BlueprintReadWrite)
	bool bRegistered;
	
	UPROPERTY(BlueprintReadWrite)
	bool bClientSideOnly;
	
	UPROPERTY(BlueprintReadWrite)
	uint8 Priority;
};


UCLASS()
class ULegacySeqAct_FinishSequence: public ULegacySequenceAction
{
	GENERATED_BODY()
public:
	
	virtual UEdGraphPin*			GetInputPin			(int32 Index,UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UEdGraphPin*			GetEventPin			(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UK2Node*				ExportToBlueprint	(UBlueprint* InBlueprint, UEdGraph* InGraph) override;
	
	UPROPERTY(Transient)
	FName InputPinName;
	
	UPROPERTY(Transient)
	UK2Node* CurrentTunnelNode = nullptr;
};

UCLASS()
class ULegacySeqEvent_SequenceActivated: public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	
	virtual UEdGraphPin*			GetInputPin			(int32 Index,UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UEdGraphPin*			GetEventPin			(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UK2Node*				ExportToBlueprint	(UBlueprint* InBlueprint, UEdGraph* InGraph) override;
	
	UPROPERTY(Transient)
	FName OutputPinName;
	
	UPROPERTY(Transient)
	UK2Node* CurrentTunnelNode = nullptr;
};


UCLASS()
class ULegacySequenceVariable : public ULegacySequenceObject
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph);
	virtual void						Fill					(ALegacyKismet* Kismet);
	
	UPROPERTY(BlueprintReadWrite)
	FName VarName;

	UPROPERTY(Transient)
	FGuid VarGuid;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_Object : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet);
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* ObjValue;
	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_Bool : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet) override;
	
	UPROPERTY(BlueprintReadWrite)
	bool bValue;
	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_Int : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet) override;
	
	UPROPERTY(BlueprintReadWrite)
	int32 IntValue;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_Float : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	UPROPERTY(BlueprintReadWrite)
	float FloatValue;
	
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_Named: public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet);

	ULegacySequenceVariable*			FindVariable			();
	UPROPERTY(Transient)
	ULegacySequenceVariable* RefVariable;
	
	UPROPERTY(BlueprintReadWrite)
	FName FindVarName;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_ObjectList : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet) override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyObject*> ObjList;
	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqVar_External : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet) override;
	
	UPROPERTY(Transient)
	TArray<ULegacySequenceVariable*> Variables;
};




UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXSeqVar_PlayerController : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXSeqVar_Elizabeth : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	
};
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXSeqVar_SinglePlayerController : public ULegacyXSeqVar_PlayerController
{
	GENERATED_BODY()
};

UCLASS()
class REDUELEGACYIMPORTER_API USeqAct_ToggleCinematicMode : public ULegacySequenceAction
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisableMovement = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisableTurning  = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHidePlayer = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisableInput = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHideHUD = true;
	
	UPROPERTY(BlueprintReadWrite)
	bool bDisableAI;
	
	UPROPERTY(BlueprintReadWrite)
	bool bHideFlashlight;
	
	UPROPERTY(BlueprintReadWrite)
	bool bForMovieCapture;
	
	UPROPERTY(BlueprintReadWrite)
	bool bLowerWeapon;
	
	UPROPERTY(BlueprintReadWrite)
	bool bRaiseWeapon;
	
	UPROPERTY(BlueprintReadWrite)
	bool mbUncrouchPlayer;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* mCinematicMesh;
	
	UPROPERTY(BlueprintReadWrite)
	FName mEndWeapon;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacySeqAct_Interp : public ULegacySequenceImporter
{
	GENERATED_BODY()
public:
									ULegacySeqAct_Interp();
	virtual UK2Node*				ExportToBlueprint	(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual UEdGraphPin*			GetInputPin			(int32 Index,UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	virtual void					SimulatedImport	() override;

	TMap<FName,FName>				EventName2FunctionName;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPatternCommand_Base: public ULegacyObject
{
	GENERATED_BODY()
public:
	
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern);
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPatternUObjectSetBase: public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual class UXPatternObjectSetBase* MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern);
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPatternUObjectSet_PlayerPawns: public ULegacyXPatternUObjectSetBase
{
	GENERATED_BODY()
public:
	virtual class UXPatternObjectSetBase* MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern) override;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPatternUObjectSet_ElizabethPawns: public ULegacyXPatternUObjectSetBase
{
	GENERATED_BODY()
public:
	virtual class UXPatternObjectSetBase* MakeObjectSet(class UXSeqAct_InstancePattern* InstancePattern) override;
};
UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPatternCommand_ActivateNamedPatternKismetEvent: public ULegacyXPatternCommand_Base
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
	
	UPROPERTY(BlueprintReadWrite)
	FName KismetPatternEventName;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPatternCommand_PlaySound: public ULegacyXPatternCommand_Base
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
class REDUELEGACYIMPORTER_API ULegacyXPatternCommand_PlaySpeech: public ULegacyXPatternCommand_Base
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
class REDUELEGACYIMPORTER_API ULegacyXPatternEvent_TimeElapsed: public ULegacyXPatternCommand_Base
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
class REDUELEGACYIMPORTER_API ULegacyXPatternEvent_AudioTimeElapsed: public ULegacyXPatternEvent_TimeElapsed
{
	GENERATED_BODY()
public:
	virtual class UXPatternCommandBase* MakeCommand(class UXSeqAct_InstancePattern* InstancePattern) override;
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXPattern_AutomaticallyRun : public ULegacyObject
{
	GENERATED_BODY()
public:
	virtual void ExportToInstancePattern(class UXSeqAct_InstancePattern* InstancePattern);
	
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyXPatternCommand_Base*> Sequence;
};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXSeqAct_InstancePattern : public ULegacySequenceImporter
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
class REDUELEGACYIMPORTER_API ULegacySeqAct_SetMaterial : public ULegacySequenceImporter
{
	GENERATED_BODY()
public:
									ULegacySeqAct_SetMaterial	();
	virtual void					FillAction					(USequenceAction* InSequenceAction) override;
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* NewMaterial;

	UPROPERTY(BlueprintReadWrite)
	int32 MaterialIndex = 0;

};


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyXSeqAct_PlaySound : public ULegacySequenceImporter
{
	GENERATED_BODY()
public:
									ULegacyXSeqAct_PlaySound	();
	virtual void					FillAction					(USequenceAction* InSequenceAction) override;
	
	UPROPERTY(BlueprintReadWrite)
	class ULegacyXEffectSound* PlaySound;
};


UCLASS()
class REDUELEGACYIMPORTER_API  ULegacyInterpTrack: public ULegacyObject
{
	
public:
	GENERATED_BODY()
	
	virtual void ExportToLevelSequence( const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction);
};

USTRUCT(BlueprintType)
struct FLegacyEventTrackKey
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	float Time;
	
	UPROPERTY(BlueprintReadWrite)
	FName EventName;
};

UCLASS()
class REDUELEGACYIMPORTER_API  ULegacyInterpTrackEvent: public ULegacyInterpTrack
{
	GENERATED_BODY()
public:
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyEventTrackKey> EventTrack;
};
UENUM()
enum class ETrackToggleAction
{
	ETTA_Off,
	ETTA_On,
	ETTA_Toggle,
	ETTA_Trigger
};


USTRUCT(BlueprintType)
struct FLegacyToggleTrackKey
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	float Time = 0;
	
	UPROPERTY(BlueprintReadWrite)
	ETrackToggleAction ToggleAction = ETrackToggleAction::ETTA_Off;
};

UCLASS()
class REDUELEGACYIMPORTER_API  ULegacyInterpTrackToggle: public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyToggleTrackKey> ToggleTrack;
};

UENUM()
enum class EVisibilityTrackAction
{
	EVTA_Hide,
	EVTA_Show,
	EVTA_Toggle
};


USTRUCT(BlueprintType)
struct FLegacyVisibilityTrackKey
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	float Time = 0;
	
	UPROPERTY(BlueprintReadWrite)
	EVisibilityTrackAction Action = EVisibilityTrackAction::EVTA_Hide;
};

UCLASS()
class REDUELEGACYIMPORTER_API  ULegacyInterpTrackVisibility: public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyVisibilityTrackKey> VisibilityTrack;
};


USTRUCT(Blueprintable)
struct FLegacyAnimControlTrackKey
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	float StartTime = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	FName AnimSeqName;
	
	UPROPERTY(BlueprintReadWrite)
	float AnimStartOffset = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float AnimEndOffset = 0.0f;
	
	UPROPERTY(BlueprintReadWrite)
	float AnimPlayRate  = 1.0f;
	
	UPROPERTY(BlueprintReadWrite)
	bool bLooping = false;
	
	UPROPERTY(BlueprintReadWrite)
	bool bReverse = false;

};

UCLASS()
class ULegacyInterpTrackAnimControl : public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FLegacyAnimControlTrackKey> AnimSeqs;

};

USTRUCT(Blueprintable)
struct FInterpCurveVector3fPoint
{
	GENERATED_BODY()
	
	/** Float input value that corresponds to this key (eg. time). */
	UPROPERTY(BlueprintReadWrite)
	float		InVal;

	/** Output value of templated type when input is equal to InVal. */
	UPROPERTY(BlueprintReadWrite)
	FVector3f	OutVal;

	/** Tangent of curve arrive this point. */
	UPROPERTY(BlueprintReadWrite)
	FVector3f	ArriveTangent; 

	/** Tangent of curve leaving this point. */
	UPROPERTY(BlueprintReadWrite)
	FVector3f	LeaveTangent; 

	/** Interpolation mode between this point and the next one. @see EInterpCurveMode */
	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EInterpCurveMode>	InterpMode; 
};

USTRUCT(Blueprintable)
struct FInterpCurveVector3f
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FInterpCurveVector3fPoint> Points;
};


UENUM()
enum class ELegacyInterpTrackMoveFrame
{
	IMF_World,                      // 0
	IMF_RelativeToInitial,          // 1
	IMF_MAX                         // 2
};

UCLASS()
class ULegacyInterpTrackMove : public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FInterpCurveVector3f PosTrack;
	
	UPROPERTY(BlueprintReadWrite)
	FInterpCurveVector3f EulerTrack;
	
	UPROPERTY(BlueprintReadWrite)
	ELegacyInterpTrackMoveFrame MoveFrame = ELegacyInterpTrackMoveFrame::IMF_World;
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
	
};


UCLASS()
class ULegacyInterpTrackFloatMaterialParam : public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParamName;
	
	UPROPERTY(BlueprintReadWrite)
	FInterpCurveFloat FloatTrack;
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
};

UCLASS()
class ULegacyInterpTrackFloatParticleParam : public ULegacyInterpTrack
{
public:
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FName ParamName;
	
	UPROPERTY(BlueprintReadWrite)
	FInterpCurveFloat FloatTrack;
	
	virtual void ExportToLevelSequence(const TSharedRef<ISequencer>&Sequencer,ULegacyActor* LegacyAction) override;
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


UCLASS()
class REDUELEGACYIMPORTER_API ULegacyInterpGroup : public ULegacyObject
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyInterpTrack*> InterpTracks;
	
	UPROPERTY(BlueprintReadWrite)
	FName GroupName;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class ULegacyAnimSet*> GroupAnimSets;

	
	void			ExportToLevelSequence	(ULegacySeqAct_Interp* OwnerSeqAction ,const TSharedRef<ISequencer>&Sequencer);
	UAnimSequence*	FindAnimSequence			(const FName&InName,USkeleton*Skeleton);
};

UCLASS()
class REDUELEGACYIMPORTER_API ULegacyInterpData : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	
	virtual FName						GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	virtual void						Fill					(ALegacyKismet* Kismet) override;
			ULevelSequence*				GetOrCreateLevel		();
			void						ExportToLevelSequence	(ULegacySeqAct_Interp* OwnerSeqAction ,ULevelSequence* LevelSequence);
			FGuid						FindOrCreateBinding		(AActor& ActorToBind,const FString&NameBinding);
	
			FGuid						FindOrCreateBinding		(USceneComponent& ComponentToBind,const FString&NameBinding);
			template<typename T>
			T*							FindOrCreateTrack		(FGuid ObjectGuid, const FName& TrackName = NAME_None)
			{
				UMovieScene* MovieScene = LevelSequence->GetMovieScene();
				if (!MovieScene)
				{
					return nullptr;
				}
				if (ObjectGuid.IsValid())
				{
					if (T*Result =  MovieScene->FindTrack<T>(ObjectGuid,TrackName))
					{
						return Result;
					}
				}
				return MovieScene->AddTrack<T>(ObjectGuid);
			}
			template<typename T>
			T*							CreateTrack		()
			{
				UMovieScene* MovieScene = LevelSequence->GetMovieScene();
				if (!MovieScene)
				{
					return nullptr;
				}
				return MovieScene->AddTrack<T>();
			}
	
	UPROPERTY(BlueprintReadWrite)
	float InterpLength;

	UPROPERTY(BlueprintReadWrite)
	TArray<ULegacyInterpGroup*> InterpGroups;

	UPROPERTY(Transient)
	ULegacySeqAct_Interp* OwnerSeqAct_Interp = nullptr;

	UPROPERTY(Transient)
	ALegacyKismet* CurrentKismet;
	
private:
	
	UPROPERTY(Transient)
	ULevelSequence* LevelSequence;

	UPROPERTY(Transient)
	TMap<USceneComponent*,FGuid> BindingMap;
	
};