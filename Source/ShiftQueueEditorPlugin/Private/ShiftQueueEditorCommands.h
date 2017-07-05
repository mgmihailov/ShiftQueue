#pragma once

#include "Commands.h"
#include "EditorStyleSet.h"

class FShiftQueueEditorCommands : public TCommands<FShiftQueueEditorCommands>
{
public:

	FShiftQueueEditorCommands()
		: TCommands<FShiftQueueEditorCommands>(TEXT("ShiftQueueEditorPlugin"),
			NSLOCTEXT("Contexts", "FShiftQueueEditorCommands", "ShiftQueue Editor Plugin"), NAME_None, FEditorStyle::GetStyleSetName())
	{
	}

	virtual void RegisterCommands() override;

	struct Execute
	{
		static void AddSystem();
	};

	struct CanExecute
	{
		static bool AddSystem();
	};

	TSharedPtr<FUICommandInfo> AddSystem;
};