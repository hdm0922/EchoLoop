#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EchoLoopSubsystem.generated.h"

class AEchoCharacter;
class ALoopResettableActor;
class APlayerStart;
class UEchoRecordComponent;

struct FEchoRecord;

UCLASS()
class ECHOLOOP_API UEchoLoopSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:

	void RegisterEchoRecorder(TWeakObjectPtr<UEchoRecordComponent> InRecordComponent);
	void RegisterResettableActor(TWeakObjectPtr<ALoopResettableActor> InResettableActor);

	void UnregisterResettableActor(TWeakObjectPtr<ALoopResettableActor> InResettableActor);

protected:

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Deinitialize() override;

private:

	void Callback_TimeLimitExceeded();

	void RecordEcho();
	void ResetCharacterStatus(class ACharacter* InCharacter);
	void ResetActors();
	void ResetEntityStatus();

	void SpawnEchoCharacter();
	void StartLoopTimer();



	FTimerHandle LoopTimer;

	TArray<TWeakObjectPtr<AEchoCharacter>>		EchoCharacterArray;
	TArray<TSharedPtr<const FEchoRecord>>		EchoRecordArray;

	TSet<TWeakObjectPtr<ALoopResettableActor>>	ActorsToReset;

	TWeakObjectPtr<UEchoRecordComponent>		EchoRecorder;
	TWeakObjectPtr<APlayerStart>				PlayerStart;
};
