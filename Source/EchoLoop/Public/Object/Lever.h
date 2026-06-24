#pragma once

#include "CoreMinimal.h"
#include "Interaction/Interactable.h"
#include "Object/LoopResettableActor.h"
#include "Lever.generated.h"

UCLASS()
class ECHOLOOP_API ALever : public ALoopResettableActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	ALever();

	virtual void InteractWith(AActor* InActor) override;

protected:

	virtual void BeginPlay() override;
	
	virtual void TurnOFF() override;
	virtual void TurnON() override;

private:

	UPROPERTY(VisibleAnywhere, Category = "Lever")
	TObjectPtr<UStaticMeshComponent> LeverMesh;
};
