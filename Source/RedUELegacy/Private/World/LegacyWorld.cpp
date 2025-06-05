#include "World/LegacyWorld.h"

#include "Editor.h"
#include "EditorLevelUtils.h"
#include "FileHelpers.h"
#include "LevelUtils.h"
#include "UnrealEdGlobals.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacyArchive.h"
#include "Core/RedUELegacySubsystem.h"
#include "Editor/UnrealEdEngine.h"

void ULegacyWorld::LegacySerialize(FRedUELegacyArchive& Ar)
{
	Super::LegacySerialize(Ar);
	Ar << PersistentLevel;
	Ar.Seek(Ar.GetStopper());
}

UObject* ULegacyWorld::ExportToContent()
{
	if (PersistentLevel)
	{
		PersistentLevel->ExportToContent();
	}
	return nullptr;
}

void ULegacyWorld::ImportLevel(bool ReimportKismet)
{
	if (PersistentLevel)
	{
		PersistentLevel->ImportLevel( ReimportKismet);
	}
}

void ULegacyWorld::ImportWorld(TSet<FName> AllowLevels, TSet<FName> DenyLevels, bool AllowAlwaysLoadingLevel,bool ImportPersistentLevel,bool ReimportKismet)
{
	URedUELegacySubsystem* RedUELegacySubsystem = GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
	auto FindOrCreateLevel = [this,RedUELegacySubsystem](FName InLevelName, bool& NeedOverride)-> ULevel*
	{
		UWorld* WorkingWorld = GWorld;
		FString LevelPackageName = FPaths::Combine(RedUELegacySubsystem->OutContentPath, InLevelName.ToString());

		if (ULevelStreaming* StreamingLevel = FLevelUtils::FindStreamingLevel(WorkingWorld, *LevelPackageName))
		{
			if (!StreamingLevel->GetLoadedLevel())
			{
				WorkingWorld->LoadSecondaryLevels();
				if (!ensure(StreamingLevel->GetLoadedLevel()))
				{
					return nullptr;
				}
			}

			if (ULevel* Level = StreamingLevel->GetLoadedLevel())
			{
				NeedOverride = true;
				return Level;
			}
		}

		ULevel* CurrentLevel = WorkingWorld->PersistentLevel;

		// The level is not part of the world
		FString PackageFilename;
		FPackageName::TryConvertLongPackageNameToFilename(LevelPackageName, PackageFilename, FPackageName::GetMapPackageExtension());
		FSoftObjectPath LevelSoftObjectPath(LevelPackageName + TEXT(".") + InLevelName.ToString());

		ULevelStreaming* StreamingLevel = nullptr;
		// If it already exists on disk or in memory, add it to the working world
		if (FPaths::FileExists(PackageFilename) || LevelSoftObjectPath.ResolveObject())
		{
			FTransform LevelTransform;
			StreamingLevel = UEditorLevelUtils::AddLevelToWorld(WorkingWorld, *LevelPackageName, ULevelStreamingAlwaysLoaded::StaticClass(), LevelTransform);
			if (StreamingLevel)
			{
				WorkingWorld->LoadSecondaryLevels();
				if (!ensure(StreamingLevel->GetLoadedLevel()))
				{
					return nullptr;
				}
			}
			else
			{
				ensure(false);
				return nullptr;
			}
			NeedOverride = true;
		}
		// If it does not exist, create a new one and add it to the working world
		else
		{
			StreamingLevel = EditorLevelUtils::CreateNewStreamingLevelForWorld(*WorkingWorld, ULevelStreamingAlwaysLoaded::StaticClass(), PackageFilename, false, nullptr, false);

			if (StreamingLevel)
			{
				// Register the newly created map asset (associated with this consumer) to the asset registry
				UPackage* WorldPackage = FindPackage(nullptr, *StreamingLevel->GetWorldAssetPackageName());
				ensure(WorldPackage);

				UWorld* World = nullptr;
				ForEachObjectWithPackage(WorldPackage, [&World](UObject* Object)
				{
					if (UWorld* WorldAsset = Cast<UWorld>(Object))
					{
						World = WorldAsset;
						return false;
					}
					return true;
				}, false);

				ensure(World);

				FAssetRegistryModule::AssetCreated(World);
			}
			else
			{
				ensure(false);
				return nullptr;
			}
		}
		if (StreamingLevel)
		{
			return StreamingLevel->GetLoadedLevel();
		}
		checkNoEntry();
		return nullptr;
	};
	if (ImportPersistentLevel)
	{
		{
			GWorld->SetCurrentLevel(GWorld->PersistentLevel);
			{
				TArray<AActor*> Actors = GWorld->PersistentLevel->Actors;
				Actors.Remove(nullptr);
				GUnrealEd->DeleteActors(Actors, GWorld, GUnrealEd->GetSelectedActors()->GetElementSelectionSet());
			}
		}
		ImportLevel(ReimportKismet);
	}
	
	for (ULegacyActor* Actor : PersistentLevel->Actors)
	{
		if (ULegacyWorldInfo* WorldInfo = Cast<ULegacyWorldInfo>(Actor))
		{
			TArray<ULegacyLevelStreaming*> StreamingLevels;
			WorldInfo->GetStreamingLevels(StreamingLevels);
			for (ULegacyLevelStreaming* StreamingLevel : StreamingLevels)
			{
				if (ensure(StreamingLevel))
				{
					bool CanImportWorld = false;
					if (AllowLevels.Num() > 0)
					{
						if (AllowLevels.Contains(StreamingLevel->PackageName) || (StreamingLevel->IsA<ULegacyLevelStreamingAlwaysLoaded>() && AllowAlwaysLoadingLevel))
						{
							CanImportWorld = true;
						}
					}
					else if (DenyLevels.Num() > 0)
					{
						CanImportWorld = !DenyLevels.Contains(StreamingLevel->PackageName);
					}
					else
					{
						CanImportWorld = true;
					}
					if (CanImportWorld)
					{
						ULegacyPackage* SubLevelLegacyPackage = RedUELegacySubsystem->GetPackage(StreamingLevel->PackageName.ToString());
						int32 TheWorldIndex = SubLevelLegacyPackage->FindExport(NAME_TheWorld);
						if (TheWorldIndex != INDEX_NONE)
						{
							if (ULegacyWorld* LegacyWorld = Cast<ULegacyWorld>(SubLevelLegacyPackage->GetOrCreateExport(TheWorldIndex)))
							{
								bool NeedOverride = false;
								if (ULevel* Level = FindOrCreateLevel(StreamingLevel->PackageName, NeedOverride))
								{
									GWorld->SetCurrentLevel(Level);
									if (NeedOverride)
									{
										TArray<AActor*> Actors = Level->Actors;
										Actors.Remove(nullptr);
										GUnrealEd->DeleteActors(Actors, GWorld, GUnrealEd->GetSelectedActors()->GetElementSelectionSet());
									}

									LegacyWorld->ImportLevel(ReimportKismet);
									FAssetRegistryModule::AssetSaved(*Level);

									bool AllMovable = false;

									if (RedUELegacySubsystem->FloatingSectionIndexTable)
									{
										for (const FLegacyXWorldFloatingSectionIndexData& MapData : RedUELegacySubsystem->FloatingSectionIndexTable->MapData)
										{
											if (MapData.LevelNames.Contains(StreamingLevel->PackageName))
											{
												AllMovable = true;
												break;
											}
										}
									}

									for (AActor* ActorSpawned : Level->Actors)
									{
										if (AllMovable&&ActorSpawned)
										{
											if (USceneComponent* SceneComponent = ActorSpawned->GetRootComponent())
											{
												SceneComponent->SetMobility(EComponentMobility::Movable);
											}
										}
									}

									EditorLevelUtils::SetLevelVisibility(Level, false, true);
								}
							}
						}
					}
				}
			}
			return;
		}
	}
}
