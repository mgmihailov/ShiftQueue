#include "ShiftQueue.h"

#include "ShiftQueueCharacterMovementComponent.h"

#include "Engine.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/AITypes.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "AI/Navigation/NavigationSystem.h"

UShiftQueueCharacterMovementComponent::UShiftQueueCharacterMovementComponent(const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bIsExecutingCommand = false;
}

void UShiftQueueCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	UPathFollowingComponent* PathComp;
	GetWorld()->GetFirstPlayerController()->InitNavigationControl(PathComp);
	PathComp->OnRequestFinished.AddUObject(this, &UShiftQueueCharacterMovementComponent::OnMoveRequestFinished);
}

void UShiftQueueCharacterMovementComponent::ExecuteShiftQueueCommand(const FVector& TargetLocation)
{
	bIsExecutingCommand = true;
	UNavigationSystem* NavSys = GetWorld()->GetNavigationSystem();
	AAIController* AIController = nullptr;
	for (TActorIterator<AAIController> It(GetWorld()); It; ++It)
	{
		AIController = *It;
		break;
	}

	NavSys->SimpleMoveToLocation(GetWorld()->GetFirstPlayerController(), TargetLocation);
}

bool UShiftQueueCharacterMovementComponent::IsExecutingCommand() const
{
	return bIsExecutingCommand;
}

void UShiftQueueCharacterMovementComponent::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& PFollowingResult)
{
	if (PFollowingResult.IsSuccess())
	{
		bIsExecutingCommand = false;
	}
}