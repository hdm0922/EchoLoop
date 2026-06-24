#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/Interactable.h"
#include "Lever.generated.h"

UCLASS()
class ECHOLOOP_API ALever : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	

	ALever();

	virtual void InteractWith(AActor* InActor) override;

protected:

	virtual void BeginPlay() override;
	
private:

	void TurnOFF();
	void TurnON();

	UPROPERTY(VisibleAnywhere, Category = "Lever")
	TObjectPtr<UStaticMeshComponent> LeverMesh;

	UPROPERTY(EditAnywhere, Category = "Lever")
	TObjectPtr<UMaterialInterface> OnMaterial;

	UPROPERTY(EditAnywhere, Category = "Lever")
	TObjectPtr<UMaterialInterface> OffMaterial;

	bool bIsOn = false;
};
