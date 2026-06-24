#pragma once

#include "CoreMinimal.h"

struct FEchoRecordFrame
{
	double MoveX		= 0.0;
	double MoveY		= 0.0;
	double ControlYaw	= 0.0;
};

struct FEchoRecord
{
	TArray<FEchoRecordFrame> FrameMovement;
	TArray<uint32> JumpCommand;
};