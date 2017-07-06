#pragma once

#include "SlateBasics.h"
#include "SCompoundWidget.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FShiftQueueInputHandlerMouseButtonDown, const FVector&);

class SShiftQueueInputHandler : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SShiftQueueInputHandler) {}
	SLATE_ARGUMENT(TWeakObjectPtr<AActor>, Owner)
	SLATE_END_ARGS()

	FShiftQueueInputHandlerMouseButtonDown OnShiftQueueInputHandlerMouseButtonDown;

	SHIFTQUEUE_API void Construct(const FArguments& InArgs);

	SHIFTQUEUE_API virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	SHIFTQUEUE_API virtual bool SupportsKeyboardFocus() const override;

private:
	bool bIsShiftDown;

	TWeakObjectPtr<AActor> OwningActor;
};