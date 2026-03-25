#pragma once

class FLegacyKismetGraphEditorCommands : public TCommands<FLegacyKismetGraphEditorCommands>
{
public:
											FLegacyKismetGraphEditorCommands	();
	virtual void							RegisterCommands					() override;
	TSharedPtr< FUICommandInfo > 			RemoveEventPin;
};
