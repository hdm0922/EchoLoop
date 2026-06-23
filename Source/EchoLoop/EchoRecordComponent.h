#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EchoRecordComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOLOOP_API UEchoRecordComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEchoRecordComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	virtual void BeginPlay() override;

private:

	void RecordFixedTick();
	void ResetRecord();

	float AccumTime = 0.0f;
};
