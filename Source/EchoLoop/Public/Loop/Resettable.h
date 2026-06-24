#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Resettable.generated.h"

UINTERFACE(MinimalAPI)
class UResettable : public UInterface
{
	GENERATED_BODY()
};

class ECHOLOOP_API IResettable
{
	GENERATED_BODY()

public:

	virtual void Reset() = 0;
};
