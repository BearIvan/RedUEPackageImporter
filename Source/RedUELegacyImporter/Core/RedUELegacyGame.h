#pragma once

enum class ERedUELegacyGame:uint32
{
    Unkown          = 0,
    UE1				= 0x0100000,
    UE2				= 0x0200000,
	VENGEANCE		= 0x0210000,	// variant of UE2
    Bioshock,
	UE2X			= 0x0400000,
    UE3             = 0x0800000,
    Singularity,
    Bioshock3,
    Engine          = 0xFFF0000,
};
ENUM_CLASS_FLAGS(ERedUELegacyGame)

UENUM(BlueprintType)
enum class ERedUELegacyGameType:uint8
{
    Bioshock,
    Singularity,
    Bioshock3,
    Unkown,
};

UENUM(BlueprintType)
enum class ERedUELegacyEngineType:uint8
{
    Unkown,
    UnrealEngine1,
    UnrealEngine2,
    Vengeance,
    UnrealEngine2X,
    UnrealEngine3
};
