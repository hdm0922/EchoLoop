#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EchoLoopSubsystem.generated.h"

UCLASS()
class ECHOLOOP_API UEchoLoopSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void RegisterEchoRecorder(class UEchoRecordComponent* EchoRecordComponent);

protected:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

private:

	void Callback_TimeLimitExceeded();

	void RecordEcho();
	void ResetCharacterStatus(class ACharacter* InCharacter);
	void ResetEntityStatus();

	void StartLoopTimer();



	FTimerHandle LoopTimer;

	TArray<TSharedPtr<const struct FEchoRecord>> EchoRecordArray;

	TWeakObjectPtr<class UEchoRecordComponent> EchoRecorder;
	TWeakObjectPtr<class APlayerStart> PlayerStart;
};
