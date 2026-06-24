#pragma once

#include "CoreMinimal.h"
#include "Object/LoopResettableActor.h"
#include "PressurePlate.generated.h"

UCLASS()
class ECHOLOOP_API APressurePlate : public ALoopResettableActor
{
	GENERATED_BODY()
	
public:	

	APressurePlate();

protected:

	virtual void BeginPlay() override;

	virtual void TurnOFF() override;
	virtual void TurnON() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	void UpdateState();

	UPROPERTY(VisibleAnywhere, Category = "Pressure Plate")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Pressure Plate")
	TObjectPtr<UStaticMeshComponent> PlateMesh;

	TSet<TWeakObjectPtr<AActor>> ActorsPressing;
};
