#pragma once

#include "Components/ActorComponent.h"

#include "ShiftQueueAIMovementComponent.generated.h"

UCLASS(ClassGroup = (ShiftQueue), BlueprintType, meta = (BlueprintSpawnableComponent, ShortTooltip = "A Shift Queue AI Movement Component is a component that allows AI pawns execute move commands in a sequence, without one aborting the other."))
class SHIFTQUEUE_API UShiftQueueAIMovementComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UShiftQueueAIMovementComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// UActorComponent
	virtual void BeginPlay();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);
	// UActorComponent

	void ExecuteMoveRequest(const struct FAIMoveRequest& MoveRequest);
	bool IsExecutingRequest() const;
	bool BelongsToGroup(const FString& GroupName) const;

private:
	void OnMoveRequestFinished(struct FAIRequestID RequestID, const struct FPathFollowingResult& PFollowingResult);

	UPROPERTY(EditAnywhere)
	FString Group;

	bool bIsExecutingCommand;

};