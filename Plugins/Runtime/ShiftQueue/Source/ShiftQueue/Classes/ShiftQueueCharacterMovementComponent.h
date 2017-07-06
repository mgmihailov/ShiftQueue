#pragma once

#include "Components/ActorComponent.h"

#include "ShiftQueueCharacterMovementComponent.generated.h"

UCLASS(ClassGroup = (ShiftQueue), BlueprintType, meta = (BlueprintSpawnableComponent, ShortTooltip = "A Shift Queue Character Movement Component is a component that allows the player character to execute move commands in a sequence, without one aborting the other."))
class SHIFTQUEUE_API UShiftQueueCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UShiftQueueCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UActorComponent
	virtual void BeginPlay();
	// UActorComponent

	void ExecuteShiftQueueCommand(const FVector& TargetLocation);
	bool IsExecutingCommand() const;

private:
	void OnMoveRequestFinished(struct FAIRequestID RequestID, const struct FPathFollowingResult& PFollowingResult);

	bool bIsExecutingCommand;

};