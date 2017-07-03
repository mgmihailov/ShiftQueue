#pragma once

#include "Components/ActorComponent.h"

#include "ShiftQueueCommand.h"

#include "ShiftQueueCharacterMovementComponent.generated.h"

UCLASS(ClassGroup = (ShiftQueue), BlueprintType, meta = (BlueprintSpawnableComponent, ShortTooltip = "A Shift Queue Character Movement Component is a component that allows the player character to execute move commands in a sequence, without one aborting the other."))
class SHIFTQUEUE_API UShiftQueueCharacterMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UShiftQueueCharacterMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UActorComponent
	virtual void BeginPlay();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
	// UActorComponent

	void ExecuteShiftQueueCommand(const FShiftQueueCommand& Command);
	bool IsExecutingCommand() const;

private:
	void OnMoveRequestFinished(struct FAIRequestID RequestID, const struct FPathFollowingResult& PFollowingResult);

	bool bIsExecutingCommand;

};