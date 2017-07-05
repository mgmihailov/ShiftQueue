#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

#include "ShiftQueueInputHandler.h"

#include "ShiftQueueSystem.generated.h"

struct FAIMoveRequest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShiftQueuePlayerCommandAdded, const FVector&, TargetLocation);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShiftQueueAICommandAdded, const FVector&, TargetLocation, const FString&, GroupName);

UCLASS(BlueprintType)
class SHIFTQUEUE_API AShiftQueueSystem : public AActor
{
	GENERATED_BODY()
public:
	AShiftQueueSystem(const FObjectInitializer& PCIP);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category=ShiftQueue, meta=(ShortTooltip = "A delegate for adding a ShiftQueue player command"))
	FShiftQueuePlayerCommandAdded OnShiftQueuePlayerCommandAdded;

	UPROPERTY(BlueprintAssignable, Category=ShiftQueue, meta=(ShortTooltip = "A delegate for adding a ShiftQueue AI command"))
	FShiftQueueAICommandAdded OnShiftQueueAICommandAdded;

	UFUNCTION(BlueprintCallable, Category=ShiftQueue, meta=(ShortToolTip = "Adds a player ShiftQueue command"))
	void AddPlayerCommand(const FVector& TargetLocation);

	void AddAICommand(const FAIMoveRequest& MoveRequest, const FString& CommandGroup);

private:
	TSharedPtr<SShiftQueueInputHandler> InputHandlerWidget;
	TArray<FVector> PlayerCommands;
	TMap<FString, TArray<FAIMoveRequest>> AICommandGroups;

};