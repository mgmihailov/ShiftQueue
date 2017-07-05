// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"

class FShiftQueueEditorPluginModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	void AddMenuBarExtension(class FMenuBarBuilder& Builder);
	static void FillShiftQMenu(class FMenuBuilder& MenuBuilder);

	void MapShiftQueueActions(TSharedPtr<class FUICommandList> PluginCommands);
};