#include "ShiftQueueEditorPlugin.h"
#include "ShiftQueueEditorCommands.h"
#include "ShiftQueueSystem.h"

#include "Editor.h"
#include "EditorViewportClient.h"
#include "Runtime/Engine/Classes/Engine/World.h"

void FShiftQueueEditorCommands::RegisterCommands()
{
#define LOCTEXT_NAMESPACE "MainShiftQueueMenu"
	UI_COMMAND(AddSystem, "Add System", "Spawns a ShiftQueue System actor in the current level.", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE
}

void FShiftQueueEditorCommands::Execute::AddSystem()
{
	if (GWorld)
	{
		auto ViewportClient = static_cast<FEditorViewportClient*>(GEditor->GetActiveViewport()->GetClient());

		AActor* SystemActor = GWorld->SpawnActor<AActor>(AShiftQueueSystem::StaticClass());
		GEditor->SelectActor(SystemActor, true, true, true);
		ViewportClient->bNeedsRedraw = true;
	}
}

bool FShiftQueueEditorCommands::CanExecute::AddSystem()
{
	return GEditor->PlayWorld == nullptr;
}