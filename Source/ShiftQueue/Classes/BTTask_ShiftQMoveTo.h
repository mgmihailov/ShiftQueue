#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

#include "BTTask_ShiftQMoveTo.generated.h"

UCLASS(config = Game)
class SHIFTQUEUE_API UBTTask_ShiftQMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_UCLASS_BODY()

public:
	EBTNodeResult::Type PerformShiftQueueMove(UBehaviorTreeComponent& OwnerComp);

private:
	// UBTTask_BlackboardBase Interface
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	// UBTTask_BlackboardBase Interface

	void HandleShiftQueueCommandExecutionFinished(struct FAIRequestID RequestID, const struct FPathFollowingResult& PathFollowingResult);
	EBlackboardNotificationResult OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID);

	FDelegateHandle BBObserverDelegateHandle;

	UPROPERTY(config, Category = Node, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AcceptableRadius;

	UPROPERTY(Category = Blackboard, EditAnywhere, AdvancedDisplay, meta = (ClampMin = "1", UIMin = "1", EditCondition = "bObserveBlackboardValue"))
	float ObservedBlackboardValueTolerance;

	/** if move goal in BB changes the move will be redirected to new location */
	UPROPERTY()
	uint32 bObserveBlackboardValue : 1;

	UPROPERTY(Category = Node, EditAnywhere)
	uint32 bAllowStrafe : 1;

	/** if set, use incomplete path when goal can't be reached */
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	uint32 bAllowPartialPath : 1;

	/** if set, path to goal actor will update itself when actor moves */
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	uint32 bTrackMovingGoal : 1;

	/** if set, goal location will be projected on navigation data (navmesh) before using */
	UPROPERTY(Category = Node, EditAnywhere, AdvancedDisplay)
	uint32 bProjectGoalLocation : 1;

	/** if set to true agent's radius will be added to AcceptableRadius for purposes of checking
	*	if path's end point has been reached. Will result in AI stopping on contact with destination location*/
	UPROPERTY(Category = Node, EditAnywhere)
	uint32 bStopOnOverlap : 1;

	/** if set, move will use pathfinding. Not exposed on purpose, please use BTTask_MoveDirectlyToward */
	uint32 bUsePathfinding : 1;

	/** set automatically if move should use GameplayTasks */
	uint32 bUseGameplayTasks : 1;

	/** the group of ShiftQueue AI components that the command is aimed for */
	UPROPERTY(EditAnywhere)
	FString Group;

	FVector PreviousLocation;

	bool bWasTaskSuccessful;

};