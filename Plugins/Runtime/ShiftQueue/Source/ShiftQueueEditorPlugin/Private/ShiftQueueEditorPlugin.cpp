// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ShiftQueueEditorPlugin.h"
#include "ShiftQueueEditorCommands.h"

#include "MultiBoxExtender.h"
#include "MultiBoxBuilder.h"
#include "GenericCommands.h"
#include "LevelEditor.h"

#define LOCTEXT_NAMESPACE "FShiftQueueEditorPluginModule"

void FShiftQueueEditorPluginModule::StartupModule()
{
	FGenericCommands::Register(); // Required for the successful load of FLevelEditorModule
	FShiftQueueEditorCommands::Register();
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	TSharedRef<FExtender> MyExtender = MakeShareable(new FExtender);
	TSharedPtr<FUICommandList> PluginCommands = MakeShareable(new FUICommandList);
	MapShiftQueueActions(PluginCommands);

	TSharedRef<const FExtensionBase> MyExtension = MyExtender->AddMenuBarExtension("Window",
		EExtensionHook::After, PluginCommands,
		FMenuBarExtensionDelegate::CreateRaw(this, &FShiftQueueEditorPluginModule::AddMenuBarExtension));
	
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	TSharedPtr<FExtensibilityManager> ExtensibilityManager = LevelEditorModule.GetMenuExtensibilityManager();
	ExtensibilityManager->AddExtender(MyExtender);
}

void FShiftQueueEditorPluginModule::AddMenuBarExtension(FMenuBarBuilder& MenuBarBuilder)
{
	MenuBarBuilder.AddPullDownMenu(
		LOCTEXT("ShiftQueueMenu", "ShiftQueue"),
		LOCTEXT("ShiftQueueMenu_ToolTip", "Open the ShiftQueue menu"),
		FNewMenuDelegate::CreateStatic(&FShiftQueueEditorPluginModule::FillShiftQMenu),
		"ShiftQueue");
}

void FShiftQueueEditorPluginModule::FillShiftQMenu(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.BeginSection("ShiftQueue", LOCTEXT("ShiftQueueMenu_Heading", "ShiftQueue"));
	{
		MenuBuilder.AddMenuEntry(FShiftQueueEditorCommands::Get().AddSystem);
	}
	MenuBuilder.EndSection();
}

void FShiftQueueEditorPluginModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

void FShiftQueueEditorPluginModule::MapShiftQueueActions(TSharedPtr<FUICommandList> PluginCommands)
{
	PluginCommands->MapAction(FShiftQueueEditorCommands::Get().AddSystem,
		FExecuteAction::CreateStatic(&FShiftQueueEditorCommands::Execute::AddSystem),
		FCanExecuteAction::CreateStatic(&FShiftQueueEditorCommands::CanExecute::AddSystem));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FShiftQueueEditorPluginModule, ShiftQueueEditorPlugin)