#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EchoReplayComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOLOOP_API UEchoReplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UEchoReplayComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeReplayComponent(TSharedPtr<const struct FEchoRecord> InEchoRecord);

	void StartReplay();

protected:

	virtual void BeginPlay() override;

private:

	void ReplayEchoFrame();

	float AccumTime = 0.0f;
	uint32 TickID	= 0u;
	uint32 JumpIdx	= 0u;

	bool bJumpCommandLastFrame = false;

	TSharedPtr<const struct FEchoRecord> EchoRecord;

};
