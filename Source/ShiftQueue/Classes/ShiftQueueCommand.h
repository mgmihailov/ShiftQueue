#pragma once

#include "ShiftQueueCommand.generated.h"

UENUM()
enum EShiftQueueActionType
{
	ESQAT_Move,
	ESQAT_Invalid
};

UENUM()
enum EShiftQueueCommandType
{
	ESQCT_Character,
	ESQCT_AI,
	ESQCT_Invalid
};

USTRUCT(BlueprintType)
struct FShiftQueueCommand
{
public:
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector TargetLocation;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EShiftQueueActionType> ActionType;

	UPROPERTY(BlueprintReadWrite)
	TEnumAsByte<EShiftQueueCommandType> CommandType;

	FShiftQueueCommand() : TargetLocation(FVector::ZeroVector)
		, ActionType(EShiftQueueActionType::ESQAT_Invalid)
		, CommandType(EShiftQueueCommandType::ESQCT_Invalid)
	{
	};

	FShiftQueueCommand(const FVector& TargetLocation, const EShiftQueueActionType ActionType, const EShiftQueueCommandType CommandType)
		: TargetLocation(TargetLocation)
		, ActionType(ActionType)
		, CommandType(CommandType)
	{
	};
};