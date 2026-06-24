#pragma once

#include "CoreMinimal.h"

struct FEchoRecordFrame
{
	double MoveX		= 0.0;
	double MoveY		= 0.0;
	double ControlYaw	= 0.0;
};

struct FEchoInteractionCommand
{
	uint32							TickID				= 0u;
	TWeakObjectPtr<class AActor>	InteractableActor	= nullptr;
};

struct FEchoRecord
{
	TArray<FEchoRecordFrame>		FrameMovement;
	TArray<FEchoInteractionCommand> InteractionCommand;
	TArray<uint32>					JumpCommand;
};