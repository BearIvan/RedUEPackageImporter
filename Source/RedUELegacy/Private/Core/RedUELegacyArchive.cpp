#include "Core/RedUELegacyArchive.h"
#include "Core/LegacyPackage.h"

void FRedUELegacyArchive::DetectGame()
{
	if ( (LegacyVer == 141 && (LegacyLicenseeVer == 56 || LegacyLicenseeVer == 57)) || 	 (LegacyVer == 142 && LegacyLicenseeVer == 56) ||	 (LegacyVer == 143 && LegacyLicenseeVer == 59) )					
	 	Game = ERedUELegacyGame::Bioshock;
	if (LegacyVer == 584 && LegacyLicenseeVer == 126)
		Game = ERedUELegacyGame::Singularity;
	if (LegacyVer == 727 && LegacyLicenseeVer == 75)
		Game = ERedUELegacyGame::Bioshock3;
	if (Game == ERedUELegacyGame::Unkown)
	{
		// generic or unknown engine
		if (LegacyVer < 100)
			Game = ERedUELegacyGame::UE1;
		else if (LegacyVer < 180)
			Game = ERedUELegacyGame::UE2;
		else
			Game = ERedUELegacyGame::UE3;
	}
}

int32 FRedUELegacyArchive::GetStopper()
{
    return Tell();
}

void FRedUELegacyArchive::SetStopper(int32 Pos)
{
}

void FRedUELegacyArchive::PushStopper()
{
}

void FRedUELegacyArchive::PopStopper()
{
}

FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FString& Value)
{
    int len;
    if (Ar.Game == ERedUELegacyGame::Bioshock)
    {
        Ar << AR_INDEX(len);
        len = -len;
    }
    else
    {
        if (GameUsesFCompactIndex(Ar))
            Ar << AR_INDEX(len);
        else
            Ar << len;
    }
    
    Value.Empty((len >= 0) ? len : -len);
    if (!len)
    {
        Value.GetCharArray().AddZeroed(1);
        return Ar;
    }
    if (len > 0)
    {
        TArray<char> AnsiData;
        AnsiData.AddUninitialized(len);
        Ar.Serialize(AnsiData.GetData(), len);
        AnsiData.Add(0);
        Value = ANSI_TO_TCHAR(AnsiData.GetData());
    }
    else
    {
        len = -len;
        Value.GetCharArray().AddUninitialized(len);
        Ar.Serialize(Value.GetCharArray().GetData(), len*2);
        Value.GetCharArray().Add(0);
    }
    return Ar;
}
