#pragma once
#include "RedUELegacyGame.h"

class ULegacyObject;

class FRedUELegacyCompactIndex
{
public:
    int32	Value;
    friend FArchive& operator<<(FArchive &Ar, FRedUELegacyCompactIndex &I);
};

#define AR_INDEX(intref)	(*(FRedUELegacyCompactIndex*)&(intref))

class FRedUELegacyArchive:public FArchive
{
public:

    friend FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, FString& Value);
    
    template<typename T>
    void LegacyBulkSerialize(TArray<T>&Array)
    {
        if (LegacyVer >= 453)
        {
            int32 ElementSize;
            *this << ElementSize;
            const int32 SavePos = Tell();
            *this<<Array;
            checkf(Tell() == SavePos + 4 + Array.Num() * ElementSize,TEXT("RawArray item size mismatch: expected %d, serialized %d"), ElementSize, (Tell() - SavePos) / Array.Num());
            
        }
        else
        {
            *this<<Array;
        }
    }
    template<typename ElementType, typename AllocatorType>
    void LegacySerialize(TArray<ElementType, AllocatorType>& A,ULegacyObject* Owner)
    {
        int32 SerializeNum;
        if (GameUsesFCompactIndex())
            (*this) << AR_INDEX(SerializeNum);
        else
            (*this) << SerializeNum;
    
        A.Empty(SerializeNum);
        for (int32 i=0; i<SerializeNum; i++)
        {
            A.AddDefaulted_GetRef().Serialize(*this,Owner,i);
        }
    }

    FORCEINLINE bool GameUsesFCompactIndex()
    {
        if (Game >= ERedUELegacyGame::UE3) return false;
        if (Game == ERedUELegacyGame::UE2X && LegacyVer >= 145) return false;
        return true;
    }

    
	void				DetectGame();
    virtual int32       GetStopper();
    virtual void        SetStopper(int32 Pos);
    virtual void        PushStopper();
    virtual void        PopStopper();
    
	ERedUELegacyGame	Game;
	int32				LegacyVer;
	int32				LegacyLicenseeVer;
    
};



template<typename ElementType, typename AllocatorType>
FRedUELegacyArchive& operator<<(FRedUELegacyArchive& Ar, TArray<ElementType, AllocatorType>& A)
{
    int32 SerializeNum;
    if (Ar.GameUsesFCompactIndex())
        Ar << AR_INDEX(SerializeNum);
    else
        Ar << SerializeNum;
    
    A.Empty(SerializeNum);
    for (int32 i=0; i<SerializeNum; i++)
    {
        Ar << A.AddDefaulted_GetRef();
    }
	return Ar;
}
