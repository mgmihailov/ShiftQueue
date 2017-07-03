#include "ShiftQueue.h"

#include "BTTask_ShiftQMoveTo.h"
#include "ShiftQueueSystem.h"

#include "Engine/World.h"
#include "EngineUtils.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

UBTTask_ShiftQMoveTo::UBTTask_ShiftQMoveTo(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Shift Queue Move To";
	bNotifyTick = true;
	bNotifyTaskFinished = true;

	AcceptableRadius = GET_AI_CONFIG_VAR(AcceptanceRadius);
	bStopOnOverlap = GET_AI_CONFIG_VAR(bFinishMoveOnGoalOverlap);
	bAllowStrafe = GET_AI_CONFIG_VAR(bAllowStrafing);
	bAllowPartialPath = GET_AI_CONFIG_VAR(bAcceptPartialPaths);
	bTrackMovingGoal = true;
	bProjectGoalLocation = true;
	bUsePathfinding = true;

	ObservedBlackboardValueTolerance = AcceptableRadius * 0.95f;

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_ShiftQMoveTo, BlackboardKey));

	Group = FString("None");
	PreviousLocation = FAISystem::InvalidLocation;
}

EBTNodeResult::Type UBTTask_ShiftQMoveTo::PerformShiftQueueMove(UBehaviorTreeComponent& OwnerComp)
{
	UPathFollowingComponent* PFollowingComp = OwnerComp.GetAIOwner()->GetPathFollowingComponent();
	const UBlackboardComponent* MyBlackboard = OwnerComp.GetBlackboardComponent();
	AShiftQueueSystem* ShiftQueueSys = nullptr;

	for (TActorIterator<AShiftQueueSystem> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		ShiftQueueSys = *ActorItr;
		break;
	}

	if (!ShiftQueueSys)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}

	FVector KeyValue = MyBlackboard->GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());
	if (FVector::Distance(KeyValue, PreviousLocation) > AcceptableRadius)
	{
		FAIMoveRequest MoveReq;
		MoveReq.SetAllowPartialPath(bAllowPartialPath);
		MoveReq.SetAcceptanceRadius(AcceptableRadius);
		MoveReq.SetCanStrafe(bAllowStrafe);
		MoveReq.SetReachTestIncludesAgentRadius(bStopOnOverlap);
		MoveReq.SetProjectGoalLocation(bProjectGoalLocation);
		MoveReq.SetUsePathfinding(bUsePathfinding);
		MoveReq.SetGoalLocation(KeyValue);

		ShiftQueueSys->AddAICommand(MoveReq, Group);
		PreviousLocation = KeyValue;
	}

	if (!PFollowingComp->OnRequestFinished.IsBoundToObject(this))
	{
		PFollowingComp->OnRequestFinished.AddUObject(this, &UBTTask_ShiftQMoveTo::HandleShiftQueueCommandExecutionFinished);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ShiftQMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type NodeResult = PerformShiftQueueMove(OwnerComp);
	if (NodeResult == EBTNodeResult::InProgress && bObserveBlackboardValue)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp))
		{
			if (BBObserverDelegateHandle.IsValid())
			{
				BlackboardComp->UnregisterObserver(BlackboardKey.GetSelectedKeyID(), BBObserverDelegateHandle);
			}
			BBObserverDelegateHandle = BlackboardComp->RegisterObserver(BlackboardKey.GetSelectedKeyID(), this, FOnBlackboardChangeNotification::CreateUObject(this, &UBTTask_ShiftQMoveTo::OnBlackboardValueChange));
		}
	}

	return NodeResult;
}

EBTNodeResult::Type UBTTask_ShiftQMoveTo::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TO DO: Implement logic to be able to cancel the corresponding command in the shift queue system
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_ShiftQMoveTo::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	if (bObserveBlackboardValue)
	{
		UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
		if (ensure(BlackboardComp) && BBObserverDelegateHandle.IsValid())
		{
			BlackboardComp->UnregisterObserver(BlackboardKey.GetSelectedKeyID(), BBObserverDelegateHandle);
		}

		BBObserverDelegateHandle.Reset();
	}
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_ShiftQMoveTo::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (bWasTaskSuccessful)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		bWasTaskSuccessful = false;
	}
}

//void UBTTask_ShiftQMoveTo::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
//{
//}

void UBTTask_ShiftQMoveTo::HandleShiftQueueCommandExecutionFinished(FAIRequestID RequestID, const FPathFollowingResult& PathFollowingResult)
{
	bWasTaskSuccessful = PathFollowingResult.IsSuccess();
}

EBlackboardNotificationResult UBTTask_ShiftQMoveTo::OnBlackboardValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID)
{
	UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(Blackboard.GetBrainComponent());
	if (BehaviorComp == nullptr)
	{
		return EBlackboardNotificationResult::RemoveObserver;
	}

	const EBTTaskStatus::Type TaskStatus = BehaviorComp->GetTaskStatus(this);
	if (TaskStatus != EBTTaskStatus::Active)
	{
		//UE_VLOG(BehaviorComp, LogBehaviorTree, Error, TEXT("BT MoveTo \'%s\' task observing BB entry while no longer being active!"), *GetNodeName());

		// resetting BBObserverDelegateHandle without unregistering observer since 
		// returning EBlackboardNotificationResult::RemoveObserver here will take care of that for us
		BBObserverDelegateHandle.Reset();

		return EBlackboardNotificationResult::RemoveObserver;
	}

	bool bUpdateMove = true;
	// check if new goal is almost identical to previous one
	if (BlackboardKey.SelectedKeyType == UBlackboardKeyType_Vector::StaticClass())
	{
		const FVector TargetLocation = Blackboard.GetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID());

		bUpdateMove = (FVector::DistSquared(TargetLocation, PreviousLocation) > FMath::Square(ObservedBlackboardValueTolerance));
	}

	if (bUpdateMove)
	{
		const EBTNodeResult::Type NodeResult = PerformShiftQueueMove(*BehaviorComp);
		if (NodeResult != EBTNodeResult::InProgress)
		{
			FinishLatentTask(*BehaviorComp, NodeResult);
		}
	}

	return EBlackboardNotificationResult::ContinueObserving;
}