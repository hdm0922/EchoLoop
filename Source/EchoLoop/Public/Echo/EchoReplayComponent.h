#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EchoReplayComponent.generated.h"

class AEchoCharacter;

struct FEchoRecord;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ECHOLOOP_API UEchoReplayComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UEchoReplayComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitializeReplayComponent(TSharedPtr<const FEchoRecord> InEchoRecord);

	void StartReplay();

protected:

	virtual void BeginPlay() override;

private:

	void ReplayEchoFrame();
	void ReplayEchoInteraction(AEchoCharacter& InEchoCharacter);
	void ReplayEchoJump(AEchoCharacter& InEchoCharacter);
	void ReplayEchoMovement(AEchoCharacter& InEchoCharacter);



	float AccumTime			= 0.0f;
	uint32 TickID			= 0u;
	uint32 JumpIdx			= 0u;
	uint32 InteractionIdx	= 0u;

	bool bJumpCommandLastFrame = false;

	TSharedPtr<const FEchoRecord> EchoRecord;

};
