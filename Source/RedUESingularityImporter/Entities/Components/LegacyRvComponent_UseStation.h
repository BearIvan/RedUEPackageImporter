#pragma once

#include "LegacyRvComponent_Use.h"
#include "LegacyRvComponent_UseStation.generated.h"

	
enum EStationAnimState
{
	ESAS_Inactive,                  // 0
	ESAS_Activating,                // 1
	ESAS_Active,                    // 2
	ESAS_Deactivating,              // 3
	ESAS_Triggered,                 // 4
	ESAS_TriggeredDone,             // 5
	ESAS_MAX                        // 6
};

USTRUCT(Blueprintable)
struct FSMaterialManipulation
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mParameterName;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float mInitialParameterValue = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float mDesiredParameterValue = 0.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float mInterpolationTime = 0.f;
};

/**
 * 
 */
UCLASS()
class REDUESINGULARITYIMPORTER_API ULegacyRvComponent_UseStation : public ULegacyRvComponent_Use
{
	GENERATED_BODY()
public:
	ULegacyRvComponent_UseStation();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSMaterialManipulation mActivationMaterialEffect;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FSMaterialManipulation mDeactivationMaterialEffect;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacySoundCue* mSoundActivateOther = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacySoundCue* mSoundActivate = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacySoundCue* mSoundActiveLoop = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacySoundCue* mSoundDeactivate = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbSoundStopDeactivates = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbProximityActivate = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbUseRequiresActive = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbUseStopsTMDPowers = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mActivateAnim;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mDeactivateAnim;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mActiveIdleAnim;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mTriggeredAnim;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector3f mPlayerOffset = {150,0,0};
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mCombatNode;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FName mEndCombatNode;
};
