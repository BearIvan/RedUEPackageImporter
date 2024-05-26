#pragma once
#include "RedUELegacyGame.h"

class FRedUELegacyArchive:public FArchive
{
public:
	ERedUELegacyGame	Game;
	int32			LegacyVer;
	int32			LegacyLicenseeVer;
};


template<typename ElementType, typename AllocatorType>
FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, TArray<ElementType, AllocatorType>& A)
{
	TArrayPrivateFriend::Serialize(Ar, A);
	return Ar;
}
