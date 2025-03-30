#include "RedUELegacyEditorCommands.h"
#include "Editor.h"
#include "Core/LegacyPackage.h"
#include "Core/RedUELegacySubsystem.h"

FReply SRedUEPackageExplorer::TestClicked()
{
	ULegacyPackage *Package  = NewObject<ULegacyPackage>();
	Package->LoadPackage(TEXT("E:\\SteamLibrary\\steamapps\\common\\Singularity\\RvGame\\CookedPC\\menu_blumel.xxx"));
    URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();
    RedUELegacySubsystem->RefreshClasses(Package->EngineType,Package->GameType);
    if(ULegacyObject* TestMesh = Package->GetOrCreateExport(4623))
    {
        TestMesh->ExportToContent();
    }
	return FReply::Handled();
}

FReply SRedUEPackageExplorer::TestClicked2()
{
	URedUELegacySubsystem*RedUELegacySubsystem =  GEditor->GetEditorSubsystem<URedUELegacySubsystem>();

	
	RedUELegacySubsystem->InContentPaths.Add(TEXT("F:\\SteamLibrary\\steamapps\\common\\BioShock Infinite\\XGame\\CookedPCConsole_FR"));
	RedUELegacySubsystem->InContentPaths.Add(TEXT("F:\\SteamLibrary\\steamapps\\common\\BioShock Infinite\\DLC\\DLCB\\CookedPCConsole_FR"));
	
	RedUELegacySubsystem->OutContentPath = TEXT("/Game/Bioshock");
	//RedUELegacySubsystem->GetPackage(TEXT("SP_VI_intro_Kismet_XSA"));
	ULegacyPackage *Package = RedUELegacySubsystem->GetPackage(TEXT("S_DEPT_Entrance_Geo"));

	
	// RedUELegacySubsystem->InContentPath = TEXT("E:\\SteamLibrary\\steamapps\\common\\Singularity\\RvGame\\CookedPC\\");
	// RedUELegacySubsystem->OutContentPath = TEXT("/Game/Singularity");
	// //RedUELegacySubsystem->GetPackage(TEXT("SP_VI_intro_Kismet_XSA"));
	// ULegacyPackage *Package = RedUELegacySubsystem->GetPackage(TEXT("menu_blumel"));
	//
    int32 TheWorldIndex = Package->FindExport(NAME_TheWorld);
    if(TheWorldIndex!=INDEX_NONE)
    {
        if(ULegacyObject* LegacyWorld = Package->GetOrCreateExport(TheWorldIndex))
        {
            LegacyWorld->ExportToContent();
        }
    }
	return FReply::Handled();
}

void SRedUEPackageExplorer::Construct(const FArguments& InArgs)
{
	ChildSlot
	[
		SNew(SScrollBox)
		+ SScrollBox::Slot()
		.VAlign(VAlign_Top)
		.Padding(5)
		[
			SNew(SBorder)
			.BorderBackgroundColor(FColor(192, 192, 192, 255))
			.Padding(15.0f)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("This is a tab example."))),
				SNew(SButton)
				.Text(FText::FromString(TEXT("Test")))
				.OnClicked(this,&SRedUEPackageExplorer::TestClicked2)
				// SNew(SButton)
    //             .Text(FText::FromString(TEXT("Test2")))
    //             .OnClicked(this,&SRedUEPackageExplorer::TestClicked2)
			]
		]
	];
}
