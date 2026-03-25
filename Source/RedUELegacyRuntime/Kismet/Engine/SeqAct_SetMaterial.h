#pragma once
#include "Kismet/Base/SequenceAction.h"
#include "SeqAct_SetMaterial.generated.h"

UCLASS()
class REDUELEGACYRUNTIME_API USeqAct_SetMaterial : public USequenceAction
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
