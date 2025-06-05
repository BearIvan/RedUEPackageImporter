#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "GameplaySequenceActions.generated.h"


UCLASS()
class REDUELEGACYGAME_API UXSeqAct_PlayScriptedSequence : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Initiate();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Cleanup();

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate SequenceReady;

	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 1))
	FSequenceActionDelegate SequenceCleanedUp;

	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 1))
	AActor* StartPosition;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(AActor*,StartPosition);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 2))
	AActor* LookAtActor;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(AActor*,LookAtActor);
	
	UPROPERTY(EditAnywhere,meta=(KismetExternalVariable ,LegacyIndex = 3))
	AActor* HandsMeshTargets;
	
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(AActor*,HandsMeshTargets);
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_HideHUDElement : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void HideElements();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void ShowElements();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_PatternMatcherSetGlobalFlag : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Set();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Unset();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 2))
	void Toggle();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FName> FlagNames;
};


UCLASS()
class REDUELEGACYGAME_API UXSeqAct_PlayerAbilityFilter : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void In();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_ModifyPawnMovement : public USequenceAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 0))
	void Apply();

	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyIndex = 1))
	void Remove();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};

