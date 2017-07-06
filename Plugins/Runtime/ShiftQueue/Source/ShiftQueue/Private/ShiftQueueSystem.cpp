#include "ShiftQueue.h"

#include "ShiftQueueSystem.h"
#include "ShiftQueueCharacterMovementComponent.h"
#include "ShiftQueueAIMovementComponent.h"

#include "Engine.h"
#include "SViewport.h"

#include "AIController.h"
#include "AITypes.h"

AShiftQueueSystem::AShiftQueueSystem(const FObjectInitializer& PCIP) : Super(PCIP)
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShiftQueueSystem::BeginPlay()
{
	Super::BeginPlay();

	InputHandlerWidget = SNew(SShiftQueueInputHandler).Owner(this);
	if (GEngine->IsValidLowLevel())
	{
		GEngine->GameViewport->AddViewportWidgetContent(SNew(SWeakWidget).PossiblyNullContent(InputHandlerWidget));
	}

	InputHandlerWidget->OnShiftQueueInputHandlerMouseButtonDown.AddUObject(this, &AShiftQueueSystem::AddPlayerCommand);
}

void AShiftQueueSystem::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (PlayerCommands.Num())
	{
		for (TObjectIterator<UShiftQueueCharacterMovementComponent> It; It; ++It)
		{
			AActor* Owner = It->GetOwner();

			if (!Owner)
			{
				continue;
			}

			if (!(It->IsExecutingCommand()) && It->GetOwner()->GetWorld() == GetWorld())
			{
				It->ExecuteShiftQueueCommand(PlayerCommands[0]);
				PlayerCommands.RemoveAt(0);
			}
		}
	}

	if (AICommandGroups.Num())
	{
		for (auto& Group : AICommandGroups)
		{
			if (!Group.Value.Num())
			{
				break;
			}

			UE_LOG(LogTemp, Log, TEXT("AI command count: %d"), Group.Value.Num())

			for (TObjectIterator<UShiftQueueAIMovementComponent> It; It; ++It)
			{
				AActor* Owner = It->GetOwner();

				if (!Owner)
				{
					continue;
				}

				if (!(It->IsExecutingRequest()) && Owner->GetWorld() == GetWorld()
					&& It->BelongsToGroup(Group.Key))
				{
					It->ExecuteMoveRequest(Group.Value[0]);
				}
			}
			Group.Value.RemoveAt(0);
		}
	}
}


void AShiftQueueSystem::AddPlayerCommand(const FVector& TargetLocation)
{
	PlayerCommands.Push(TargetLocation);
	OnShiftQueuePlayerCommandAdded.Broadcast(TargetLocation);
}

void AShiftQueueSystem::AddAICommand(const FAIMoveRequest& MoveRequest, const FString& CommandGroup)
{
	AICommandGroups.FindOrAdd(CommandGroup);
	AICommandGroups[CommandGroup].Add(MoveRequest);
	OnShiftQueueAICommandAdded.Broadcast(MoveRequest.GetGoalLocation(), CommandGroup);
}