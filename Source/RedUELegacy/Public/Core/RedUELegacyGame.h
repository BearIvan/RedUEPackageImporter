#pragma once

enum class ERedUELegacyGame:uint32
{
    Unkown          = 0,
    UE2				= 0x0200000,
	VENGEANCE		= 0x0210000,	// variant of UE2
    Bioshock,
	UE2X			= 0x0400000,
    UE3             = 0x0800000,
    Singularity,
    Engine          = 0xFFF0000,
};
ENUM_CLASS_FLAGS(ERedUELegacyGame)