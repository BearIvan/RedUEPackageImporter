#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "RedUEBlueprintFunctionLibrary.generated.h"

UENUM(BlueprintType)
enum class ERedUEToggleCinematicModeThen : uint8
{
	Out,
	Ready,
	Finished,
};
UENUM(BlueprintType)
enum class ERedUEToggleCinematicModeExec : uint8
{
	Enable,
	Disable,
	Toggle,
};


UENUM(BlueprintType)
enum class ELegacyXPlayScriptedSequenceThen : uint8
{
	Ready,
	CleanedUp,
};
UENUM(BlueprintType)
enum class ELegacyXPlayScriptedSequenceExec : uint8
{
	Initiate,
	CleanUp,
};

USTRUCT(Blueprintable)
struct FRedUEToggleCinematicModeParameters
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bDisableMovement = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bDisableTurning = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bHidePlayer = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bDisableInput  = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bHideHUD = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bDisableAI = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bHideFlashlight = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bForMovieCapture = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bLowerWeapon = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bRaiseWeapon = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	bool bUncrouchPlayer = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	USkeletalMesh* CinematicMesh = nullptr;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Red|ToggleCinematicMode")
	FName EndWeapon;
};

UCLASS()
class REDUELEGACYGAME_API URedUEBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, meta=(Latent, LatentInfo="LatentInfo", WorldContext="WorldContextObject", ExpandEnumAsExecs="Then,Exec"), Category="Red|UELegacy")
	static void ToggleCinematicMode(ERedUEToggleCinematicModeExec Exec,ERedUEToggleCinematicModeThen&Then, UObject* WorldContextObject, FLatentActionInfo LatentInfo ,const FRedUEToggleCinematicModeParameters&Parameters = FRedUEToggleCinematicModeParameters());

	UFUNCTION(BlueprintCallable, meta=(Latent, LatentInfo="LatentInfo", WorldContext="WorldContextObject", ExpandEnumAsExecs="Then,Exec"), Category="Red|UELegacy|Bioshock")
	static void XPlayScriptedSequence(ELegacyXPlayScriptedSequenceExec Exec,ELegacyXPlayScriptedSequenceThen&Then, UObject* WorldContextObject, FLatentActionInfo LatentInfo ,const FRedUEToggleCinematicModeParameters&Parameters = FRedUEToggleCinematicModeParameters());

};
