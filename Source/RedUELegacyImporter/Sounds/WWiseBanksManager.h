#pragma once

#include "WWiseBanksManager.generated.h"


enum class EWWiseBankObjectType : uint8
{
	SoundEffectOrVoice = 2,
	EventAction = 3,
	Event = 4,
	RandomOrSequenceContainer = 5,
	SwitchContainer = 6,
	ActorMixer = 7,
	AudioBus = 8,
	BlendContainer = 9,
	MusicSegment = 10,
	MusicTrack = 11,
	MusicSwitchContainer = 12,
	MusicPlaylistContainer = 13,
	Attenuation = 14,
	DialogueEvent = 15,
	MotionBus = 16,
	MotionFx = 17,
	Effect = 18,
	Unknown = 19,
	AuxiliaryBus = 20
};
class FWWiseBank;
class FWWiseBankObjectBase
{
public:
							FWWiseBankObjectBase	(FWWiseBank* InOwnerBank,EWWiseBankObjectType Type) : OwnerBank(InOwnerBank),Type(Type) {}
	virtual					~FWWiseBankObjectBase	() {}
	virtual USoundBase*		ExportToContent			();
	virtual void			Serialize				(FArchive& Ar,int32 Size) = 0;
	virtual bool			CanCreateInCue			() const {return false;};
	FWWiseBank*				OwnerBank;
	EWWiseBankObjectType	Type;
};

class FWWiseBankObjectUnknown:public FWWiseBankObjectBase
{
public:
							FWWiseBankObjectUnknown		(FWWiseBank* InOwnerBank,EWWiseBankObjectType Type) : FWWiseBankObjectBase(InOwnerBank,EWWiseBankObjectType::Unknown),UnknownType(Type) {}
	virtual 				~FWWiseBankObjectUnknown	() override {}
	virtual void			Serialize					(FArchive& Ar,int32 Size) override;
	
	EWWiseBankObjectType 	UnknownType;
	TArray<uint8>		 	Data;
};

class FWWiseBankObjectSoundEffectOrVoice:public FWWiseBankObjectBase
{
public:
							FWWiseBankObjectSoundEffectOrVoice		(FWWiseBank* InOwnerBank) : FWWiseBankObjectBase(InOwnerBank,EWWiseBankObjectType::Event){}
	virtual 				~FWWiseBankObjectSoundEffectOrVoice		() override {}
	virtual void			Serialize								(FArchive& Ar,int32 Size) override;
	virtual USoundBase*		ExportToContent							() override;
	virtual bool			CanCreateInCue							() const override;
	int32		 			ObjectID = -1;
};



class FWWiseBankObjectEvent:public FWWiseBankObjectBase
{
public:
							FWWiseBankObjectEvent		(FWWiseBank* InOwnerBank, int32 InEventID) : FWWiseBankObjectBase(InOwnerBank,EWWiseBankObjectType::Event), EventID(InEventID){}
	virtual 				~FWWiseBankObjectEvent		() override {}
	virtual void			Serialize					(FArchive& Ar,int32 Size) override;
	virtual USoundBase*		ExportToContent				() override;
	TArray<int32>		 	ObjectIDs;
	int32				 	EventID;
};


enum class EWWiseEventActionScope : uint8
{
	SwitchOrTrigger = 1,
	Global = 2,
	GameObject = 3,
	State = 4,
	All = 5,
	AllExcept = 6
};

enum class EWWiseEventActionType : uint8
{
	Stop = 1,
	Pause = 2,
	Resume = 3,
	Play = 4,
	Trigger = 5,
	Mute = 6,
	UnMute = 7,
	SetVoicePitch = 8,
	ResetVoicePitch = 9,
	SetVoiceVolume = 10,
	ResetVoiceVolume = 11,
	SetBusVolume = 12,
	ResetBusVolume = 13,
	SetVoiceLowPassFilter = 14,
	ResetVoiceLowPassFilter = 15,
	EnableState = 16,
	DisableState = 17,
	SetState = 18,
	SetGameParameter = 19,
	ResetGameParameter = 20,
	SetSwitch = 21,
	ToggleBypass = 22,
	ResetBypassEffect = 23,
	Break = 24,
	Seek = 25
};

enum class EWWiseEventActionParameterType : uint8
{
	Delay = 0x0E,
	Play = 0x0F,
	Probability = 0x10
};

class FWWiseBankObjectEventAction:public FWWiseBankObjectBase
{
public:
							FWWiseBankObjectEventAction		(FWWiseBank* InOwnerBank) : FWWiseBankObjectBase(InOwnerBank,EWWiseBankObjectType::EventAction) {}
	virtual 				~FWWiseBankObjectEventAction	() override {}
	virtual void			Serialize						(FArchive& Ar,int32 Size) override;
	virtual bool			CanCreateInCue					() const override;
	virtual USoundBase*		ExportToContent							() override;

	EWWiseEventActionScope Scope = EWWiseEventActionScope::All;
	EWWiseEventActionType ActionType = EWWiseEventActionType::Stop;
	int32 ObjectID = -1;
	TArray<EWWiseEventActionParameterType> ParametersTypes;
	TArray<uint8> Parameters;
};

struct FWWiseBankSound
{
	int32 Id = 0;
	int64 Offset = 0;
	int32 Size = 0;
	
	friend FArchive& operator<<(FArchive& Ar,FWWiseBankSound& BankSound);
};
class FWWiseBank
{
public:
													FWWiseBank			(class UWWiseBanksManager* InOwner);
													~FWWiseBank			();
	bool											Load				(const FString& FileName);
	USoundBase* 									ExportToContent		(int32 ObjectID);
	bool											IsValid				(int32 ObjectID) const;
	bool											IsValidSound		(int32 SoundID) const;	
	USoundBase*										CreateOrLoadSound	(int32 SoundID) const;	
	FString											BankName;
	FString											FileName;
	TMap<int32, TUniquePtr<FWWiseBankObjectBase>>	Objects;
	int32											DataOffset = INDEX_NONE;
	uint32											VersionBank = 0;
	uint32											BankId = 0;
	class UWWiseBanksManager*						Owner;				
	TMap<int32,FWWiseBankSound>						Sounds;
};

UCLASS()
class REDUELEGACYIMPORTER_API UWWiseBanksManager : public UObject
{
	GENERATED_BODY()
public:
	USoundBase*										ExportToContent				(const FString&LevelName, int32 AudioID);
	void											Empty						();
	bool											IsValidSoundFromGlobal		(int32 SoundID) const;	
	USoundBase*										CreateOrLoadSoundFromGlobal	(int32 SoundID) const;
private:
	TArray<TSharedPtr<FWWiseBank>>*					LoadBanksForLevel			(const FString&LevelNamee);
	void											LoadGlobalBanks				();
	TMap<FString, TArray<TSharedPtr<FWWiseBank>>>	WWiseBanks;
	TArray<TSharedPtr<FWWiseBank>>					GlobalBanks;
};
