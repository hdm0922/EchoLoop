#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Loop/Resettable.h"
#include "LoopResettableActor.generated.h"

UCLASS()
class ECHOLOOP_API ALoopResettableActor : public AActor, public IResettable
{
	GENERATED_BODY()
	
public:	

	ALoopResettableActor();

	virtual void Reset() override;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	virtual void TurnOFF();
	virtual void TurnON();

	TObjectPtr<UMaterialInterface> GetMaterial_ON () const { return this->OnMaterial;  }
	TObjectPtr<UMaterialInterface> GetMaterial_OFF() const { return this->OffMaterial; }

private:

	UPROPERTY(EditAnywhere, Category = "EchoLoop")
	TObjectPtr<UMaterialInterface> OnMaterial;

	UPROPERTY(EditAnywhere, Category = "EchoLoop")
	TObjectPtr<UMaterialInterface> OffMaterial;

	bool bIsOn = false;
};
