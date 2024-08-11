#include "RedUELegacyUIManager.h"

#include "WorkspaceMenuStructure.h"
#include "WorkspaceMenuStructureModule.h"
#include "RedUELegacyEditorCommands.h"
#include "ToolMenus.h"
#include "Widgets/Docking/SDockTab.h"

void FRedUELegacyUIManager::Initialize()
{
	UICommandList = MakeShareable(new FUICommandList);
	FToolMenuOwnerScoped OwnerScoped(this);

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner( FName(TEXT("ShowUELegacyExplorer")),  FOnSpawnTab::CreateRaw(this, &FRedUELegacyUIManager::CreateShowExplorerTab))
	.SetDisplayName(NSLOCTEXT("RedUEPackageImporterModule", "UELegacyExplorerTitle", "UE Legacy Explorer"))
	.SetTooltipText(NSLOCTEXT("RedUEPackageImporterModule", "UELegacyExplorerTooltipText", "Open UE Legacy Explorer tab."))
	.SetGroup(WorkspaceMenu::GetMenuStructure().GetToolsCategory());
	
}

void FRedUELegacyUIManager::Destroy()
{
	UICommandList.Reset();
}

TSharedRef<SDockTab> FRedUELegacyUIManager::CreateShowExplorerTab(const FSpawnTabArgs& Args)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SRedUEPackageExplorer)
		];
}
