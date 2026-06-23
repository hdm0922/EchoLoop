#pragma once

#include "CoreMinimal.h"

class EchoLoopConstants
{
public:

	static constexpr uint32 LOOP_DURATION		= 10u;
	static constexpr uint32 RECORD_TICK_HZ		= 60u;
	static constexpr uint32 TOTAL_TICKS			= LOOP_DURATION * RECORD_TICK_HZ;

	static constexpr double FIXED_DELTA_TIME	= 1.0 / static_cast<double>(RECORD_TICK_HZ);
};
