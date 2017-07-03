#include "ShiftQueue.h"

#include "ShiftQueueAIMovementComponent.h"

#include "Engine.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Runtime/AIModule/Classes/AITypes.h"

UShiftQueueAIMovementComponent::UShiftQueueAIMovementComponent(const FObjectInitializer& ObjectInitializer/* = FObjectInitializer::Get()*/)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	bIsExecutingCommand = false;
	Group = FString("None");
}

void UShiftQueueAIMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	RegisterComponent();

	APawn* Owner = Cast<APawn>(GetOwner());
	AAIController* OwningAIController = Cast<AAIController>(Owner->GetController());
	OwningAIController->GetPathFollowingComponent()->OnRequestFinished.AddUObject(this, &UShiftQueueAIMovementComponent::OnMoveRequestFinished);
}

void UShiftQueueAIMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShiftQueueAIMovementComponent::ExecuteMoveRequest(const FAIMoveRequest& MoveRequest)
{
	bIsExecutingCommand = true;
	APawn* Owner = Cast<APawn>(GetOwner());
	AAIController* OwningAIController = Cast<AAIController>(Owner->GetController());

	EPathFollowingRequestResult::Type RequestResult = OwningAIController->MoveTo(MoveRequest);
}

bool UShiftQueueAIMovementComponent::IsExecutingRequest() const
{
	return bIsExecutingCommand;
}

bool UShiftQueueAIMovementComponent::BelongsToGroup(const FString& GroupName) const
{
	return GroupName.Compare(Group, ESearchCase::IgnoreCase) == 0 && GroupName != FString("None");
}

void UShiftQueueAIMovementComponent::OnMoveRequestFinished(FAIRequestID RequestID, const FPathFollowingResult& PFollowingResult)
{
	bIsExecutingCommand = false;
}