#pragma once

#include "Kismet/Base/SequenceAction.h"
#include "RvSeqAct_PawnFactory.generated.h"

/**
 * 
 */
UCLASS()
class REDUESINGULARITY_API URvSeqAct_PawnFactory : public USequenceAction
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Spawn"))
	void Spawn();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Abort Spawn"))
	void AbortSpawn();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Kill All"))
	void KillAll();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Destroy All"))
	void DestroyAll();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Activate All"))
	void ActivateAll();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Respawn"))
	void Respawn();
	
	UFUNCTION(BlueprintCallable,meta=(KismetInput,LegacyName = "Prestream"))
	void Prestream();
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "All Spawned"))
	FSequenceActionDelegate AllSpawned;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "All Dead"))
	FSequenceActionDelegate AllDead;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Spawns Disabled"))
	FSequenceActionDelegate SpawnsDisabled;

	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Spawned"))
	FSequenceActionDelegate Spawned;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "# Dead"))
	FSequenceActionDelegate AnyDead;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Each Death"))
	FSequenceActionDelegate EachDeath;
	
	UPROPERTY(BlueprintAssignable,meta = (LegacyName = "Alerted"))
	FSequenceActionDelegate Alerted;
	
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Spawn Points"))
	TArray<AActor*> SpawnPoints;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(SpawnPoints);
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Last Spawned"))
	AActor* LastSpawned;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(LastSpawned);
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Move Targets"))
	TArray<AActor*> MoveTargets;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(MoveTargets);
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Force Notice"))
	AActor* ForceNoticeActor;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(ForceNoticeActor);
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "DeadLinkCount"))
	int32 DeadLinkCount;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(DeadLinkCount);
	
	
	UPROPERTY(EditAnywhere, meta = (KismetExternalVariable, LegacyName = "Squad"))
	TArray<AActor*> AmbientNodes;
	SEQUENCE_ACTION_KISMET_ATTRIBUTE(AmbientNodes);
};
