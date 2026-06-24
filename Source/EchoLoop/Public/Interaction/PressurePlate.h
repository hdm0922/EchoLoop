#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PressurePlate.generated.h"

UCLASS()
class ECHOLOOP_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	

	APressurePlate();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
					  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:

	void TurnOFF();
	void TurnON();

	void UpdateState();

	UPROPERTY(VisibleAnywhere, Category = "Pressure Plate")
	TObjectPtr<class UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, Category = "Pressure Plate")
	TObjectPtr<UStaticMeshComponent> PlateMesh;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate")
	TObjectPtr<UMaterialInterface> OnMaterial;

	UPROPERTY(EditAnywhere, Category = "Pressure Plate")
	TObjectPtr<UMaterialInterface> OffMaterial;

	TSet<TWeakObjectPtr<AActor>> ActorsPressing;

	bool bIsOn = false;
};
