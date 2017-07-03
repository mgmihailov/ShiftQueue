#include "ShiftQueue.h"

#include "Engine.h"
#include "GameFramework/Actor.h"

#include "ShiftQueueInputHandler.h"
#include "ShiftQueueCommand.h"

void SShiftQueueInputHandler::Construct(const FArguments& InArgs)
{
	ChildSlot
		.VAlign(VAlign_Fill)
		.HAlign(HAlign_Fill);

	OwningActor = InArgs._Owner;

	bIsShiftDown = false;
}

FReply SShiftQueueInputHandler::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.IsShiftDown())
	{
		APlayerController* PlayerController = OwningActor->GetWorld()->GetFirstPlayerController();

		FHitResult CursorHit;
		PlayerController->GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
		if (CursorHit.IsValidBlockingHit())
		{
			OnShiftQueueInputHandlerMouseButtonDown.Broadcast(FShiftQueueCommand(CursorHit.ImpactPoint
				, EShiftQueueActionType::ESQAT_Move
				, EShiftQueueCommandType::ESQCT_Character));
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

bool SShiftQueueInputHandler::SupportsKeyboardFocus() const
{
	return true;
}