#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Echo/EchoRecordTypes.h"
#include "EchoRecordComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOLOOP_API UEchoRecordComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEchoRecordComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	TSharedPtr<const struct FEchoRecord> FinishRecord();

	void RecordInteractionCommand(TWeakObjectPtr<AActor> InActor);
	void RecordJumpCommand();
	void StartRecord();
	

protected:

	virtual void BeginPlay() override;

private:

	void RecordFixedTick();


	float AccumTime = 0.0f;
	uint32 TickID = 0u;

	struct FEchoRecord EchoRecord;
};
