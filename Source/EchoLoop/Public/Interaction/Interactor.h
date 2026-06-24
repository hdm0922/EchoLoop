#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactor.generated.h"

UINTERFACE(MinimalAPI)
class UInteractor : public UInterface
{
	GENERATED_BODY()
};

class ECHOLOOP_API IInteractor
{
	GENERATED_BODY()

protected:

	virtual void InteractWith(class IInteractable& InInteractable) = 0;
};
