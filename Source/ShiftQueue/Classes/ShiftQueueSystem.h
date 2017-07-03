#pragma once

#include "GameFramework/Actor.h"
#include "Runtime/AIModule/Classes/Navigation/PathFollowingComponent.h"

#include "ShiftQueueInputHandler.h"

#include "ShiftQueueSystem.generated.h"

struct FAIMoveRequest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FShiftQueuePlayerCommandAdded, const FShiftQueueCommand&, PlayerCommand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FShiftQueueAICommandAdded, const FAIMoveRequest&, AICommand, const FString&, GroupName);

UCLASS(BlueprintType)
class AShiftQueueSystem : public AActor
{
	GENERATED_BODY()
public:
	AShiftQueueSystem(const FObjectInitializer& PCIP);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(BlueprintAssignable, Category=ShiftQueue)
	FShiftQueuePlayerCommandAdded OnShiftQueuePlayerCommandAdded;

	UPROPERTY(BlueprintAssignable, Category = ShiftQueue)
	FShiftQueueAICommandAdded OnShiftQueueAICommandAdded;

	UFUNCTION(BlueprintCallable, Category=ShiftQueue)
	void AddPlayerCommand(const FShiftQueueCommand& PlayerCommand);

	void AddAICommand(const FAIMoveRequest& MoveRequest, const FString& CommandGroup);

private:
	TSharedPtr<SShiftQueueInputHandler> InputHandlerWidget;
	TArray<FShiftQueueCommand> PlayerCommands;
	TMap<FString, TArray<FAIMoveRequest>> AICommandGroups;

};