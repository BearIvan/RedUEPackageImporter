#pragma once

#include "LegacyRvActorComponent.h"
#include "LegacyRvComponent_Use.generated.h"

/**
 * 
 */
UENUM(Blueprintable)
enum class EChainActions:uint8
{
    CB_NONE,                        // 0
    CB_JUMP,                        // 1
    CB_USE,                         // 2
    CB_ATK,                         // 3
    CB_SMASH,                       // 4
    CB_DASH,                        // 5
    CB_BLOCK,                       // 6
    CB_CLAWS,                       // 7
    CB_AI_PANIC,                    // 8
    CB_STRAFE,                      // 9
    CB_LOCKON,                      // 10
    CB_AI_BLINDSEARCH,              // 11
    CB_POWER,                       // 12
    CB_FERALSENSES,                 // 13
    CB_LUNGE,                       // 14
    CB_START,                       // 15
    CB_BACK,                        // 16
    CB_POWER1,                      // 17
    CB_POWER2,                      // 18
    CB_POWER3,                      // 19
    CB_POWER4,                      // 20
    CB_POP_CLAWS,                   // 21
    CB_EVADE,                       // 22
    CB_EVADE_BIG,                   // 23
    CB_EVADE_BLOCK,                 // 24
    CB_EVADE1,                      // 25
    CB_EVADE2,                      // 26
    CB_EVADE3,                      // 27
    CB_EVADE4,                      // 28
    CB_FIRE,                        // 29
    CB_AIM,                         // 30
    CB_ANTIGRAVITY,                 // 31
    CB_CYCLEWEAPON,                 // 32
    CB_AGEOLD,                      // 33
    CB_AGEYOUNG,                    // 34
    CB_CROUCH,                      // 35
    CB_FLASHLIGHT,                  // 36
    CB_IMPULSE,                     // 37
    CB_HEALTH,                      // 38
    CB_DPAD_UP,                     // 39
    CB_DPAD_DOWN,                   // 40
    CB_DPAD_LEFT,                   // 41
    CB_DPAD_RIGHT,                  // 42
    CB_JOURNAL,                     // 43
    CB_USE_RELOAD,                  // 44
    CB_RELOAD,                      // 45
    CB_WEAPON1,                     // 46
    CB_WEAPON2,                     // 47
    CB_MAX                          // 48
};

class ULegacySoundCue;
UCLASS()
class REDUESINGULARITYIMPORTER_API ULegacyRvComponent_Use : public ULegacyRvActorComponent
{
	GENERATED_BODY()
public:
	ULegacyRvComponent_Use();
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString mPromptMarkup = TEXT("<Strings:RvGame.ActionVolume.Use>");
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float mDebounceTime = 1.f;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbUseEnabled = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbAutoUse = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mAllowContinuousTrigger = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbRequireFacing = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbRequireFacingIgnorePitch = false;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool mbRequireLOS = true;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 mMaxTriggerCount;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ULegacySoundCue* mLoopedUseSound = nullptr;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	EChainActions mUseButton = EChainActions::CB_USE;
};

