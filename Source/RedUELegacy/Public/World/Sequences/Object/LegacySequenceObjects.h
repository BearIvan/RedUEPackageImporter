#pragma once

#include "Core/LegacyObject.h"
#include "LegacySequenceObjects.generated.h"
class UK2Node;

USTRUCT(Blueprintable)
struct FLegacySeqOpInputLink
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString LinkDesc;
	
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
	TArray<class ULegacySequenceEvent*> LinkedEvents;
	
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
class REDUELEGACY_API ULegacySequenceObject : public ULegacyObject
{
	GENERATED_BODY()
};

UCLASS()
class REDUELEGACY_API ULegacySequenceOp : public ULegacySequenceObject
{
	GENERATED_BODY()
public:
	
protected:
	UPROPERTY(Transient)
	UK2Node* CurrentNode = nullptr;
	
public:
	
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
class REDUELEGACY_API ULegacySequenceAction : public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	virtual UEdGraphPin* 	GetPinOrCreateNode	(UBlueprint* InBlueprint,UEdGraph* InGraph,int32 InputLinkIdx);
	
	UPROPERTY(BlueprintReadWrite)
	FName HandlerName;
	
	UPROPERTY(BlueprintReadWrite)
	bool bCallHandler;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<class UObject*> Targets;
};


UCLASS()
class REDUELEGACY_API ULegacySequenceEvent: public ULegacySequenceOp
{
	GENERATED_BODY()
public:
	virtual UK2Node*		ExportToBlueprint	(UBlueprint* InBlueprint,UEdGraph* InGraph);
	
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
class ULegacySequenceVariable : public ULegacySequenceObject
{
	GENERATED_BODY()
public:
	virtual UK2Node*					CreateGetNode			(UBlueprint* InBlueprint,UEdGraph* InGraph);
	virtual FBPVariableDescription*		GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph);
	
	UPROPERTY(BlueprintReadWrite)
	FName VarName;

	UPROPERTY(Transient)
	FGuid VarGuid;
};

UCLASS()
class REDUELEGACY_API ULegacySeqEvent_LevelLoaded: public ULegacySequenceEvent
{
	GENERATED_BODY()
public:
	virtual UK2Node*	ExportToBlueprint(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
};

UCLASS()
class REDUELEGACY_API ULegacySeqAct_SetMatInstScalarParam : public ULegacySequenceAction
{
	GENERATED_BODY()
public:
	
	virtual UEdGraphPin* 	GetPinOrCreateNode	(UBlueprint* InBlueprint,UEdGraph* InGraph,int32 InputLinkIdx) override;
};

UCLASS()
class REDUELEGACY_API ULegacySeqVar_Object : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	virtual FBPVariableDescription*		GetOrCreateVariable		(UBlueprint* InBlueprint,UEdGraph* InGraph) override;
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* ObjValue;
	
};

UCLASS()
class REDUELEGACY_API USeqAct_ToggleCinematicMode : public ULegacySequenceAction
{
	GENERATED_BODY()
public:
	
	virtual UEdGraphPin* 	GetPinOrCreateNode	(UBlueprint* InBlueprint,UEdGraph* InGraph,int32 InputLinkIdx) override;
	
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
class REDUELEGACY_API ULegacySeqAct_Interp : public ULegacySequenceAction
{
	GENERATED_BODY()
public:
	
	virtual UEdGraphPin* 	GetPinOrCreateNode	(UBlueprint* InBlueprint,UEdGraph* InGraph,int32 InputLinkIdx) override;
};

UCLASS()
class REDUELEGACY_API ULegacyInterpData : public ULegacySequenceVariable
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadWrite)
	ULegacyObject* ObjValue;
	
};