#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "AISequenceActions.generated.h"


UCLASS()
class REDUELEGACYGAME_API UXSeqAct_BeginScriptBehavior : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;


	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 1))
	FSequenceActionDelegate Done;
};


UCLASS()
class REDUELEGACYGAME_API UXSeqAct_EndScriptBehavior : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_SmartTerrainMultiPhaseCustomAnim_Request : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void SendRequests();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> RequestsToSend;
};


UCLASS()
class REDUELEGACYGAME_API UXSeqAct_UseSmartTerrain : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 1))
	FSequenceActionDelegate Success;

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 2))
	FSequenceActionDelegate Failure;

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 3))
	FSequenceActionDelegate StartedAnim;

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 4))
	FSequenceActionDelegate LoopingAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bFindSmartTerrainUsingLabelNames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowedToFailSmartTerrain = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ForceTeleportPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SmartTerrainName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxDesiredTravelTimeSeconds = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PathDistanceWithinWhichToForceWalk;
};

UCLASS()
class REDUELEGACYGAME_API UXSeqAct_StopSmartTerrain : public USequenceAction
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void In();
	
	UFUNCTION(BlueprintCallable, meta=(KismetInput, LegacyIndex = 0))
	void RequestOutro();

	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 0))
	FSequenceActionDelegate Out;
	
	UPROPERTY(BlueprintAssignable, meta = (LegacyIndex = 1))
	FSequenceActionDelegate Done;
};