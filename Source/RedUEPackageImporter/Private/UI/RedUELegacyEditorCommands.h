#pragma once

class FRedUELegacyEditorCommands: public TCommands<FRedUELegacyEditorCommands>
{
									FRedUELegacyEditorCommands	();
	void							RegisterCommands			() override;
public:
	TSharedPtr< FUICommandInfo >	ShowUELegacyExplorer;
};
