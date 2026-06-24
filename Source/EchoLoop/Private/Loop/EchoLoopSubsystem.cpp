
#include "Loop/EchoLoopSubsystem.h"

#include "Character/EchoCharacter.h"
#include "Character/EchoLoopCharacter.h"

#include "EchoLoopUtils.h"
#include "Echo/EchoRecordComponent.h"
#include "Echo/EchoReplayComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"

#include "Loop/EchoLoopConstants.h"

#include "Object/LoopResettableActor.h"

#include "TimerManager.h"

void UEchoLoopSubsystem::RegisterEchoRecorder(TWeakObjectPtr<UEchoRecordComponent> InRecordComponent)
{
	this->EchoRecorder = InRecordComponent;

	return;
}

void UEchoLoopSubsystem::RegisterResettableActor(TWeakObjectPtr<ALoopResettableActor> InResettableActor)
{
	this->ActorsToReset.Add(InResettableActor);

	return;
}

void UEchoLoopSubsystem::UnregisterResettableActor(TWeakObjectPtr<ALoopResettableActor> InResettableActor)
{
	this->ActorsToReset.Remove(InResettableActor);

	return;
}

void UEchoLoopSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

	this->ActorsToReset			= {};
	this->EchoCharacterArray	= {};
	this->EchoRecordArray		= {};

	// Get weak reference : Player Start
	{
		AActor* pPlayerStart	= UGameplayStatics::GetActorOfClass(&InWorld, APlayerStart::StaticClass());
		this->PlayerStart		= Cast<APlayerStart>(pPlayerStart);
	}

	this->ResetEntityStatus();
	this->StartLoopTimer();

	return;
}

void UEchoLoopSubsystem::Deinitialize()
{

	if (UWorld* World = this->GetWorld())
	{
		World->GetTimerManager().ClearTimer(this->LoopTimer);
	}

	Super::Deinitialize();

	return;
}

void UEchoLoopSubsystem::Callback_TimeLimitExceeded()
{
	if (!IsValid(this->GetWorld())) return;

	this->RecordEcho();
	this->SpawnEchoCharacter();

	this->ResetEntityStatus();
	this->ResetActors();

	this->StartLoopTimer();
	
	return;
}

void UEchoLoopSubsystem::RecordEcho()
{

	UEchoRecordComponent* EchoRecordComponent = this->EchoRecorder.Get();
	if (!IsValid(EchoRecordComponent)) return;

	TSharedPtr<const FEchoRecord> CompletedRecord = EchoRecordComponent->FinishRecord();
	if (!CompletedRecord.IsValid()) return;

	this->EchoRecordArray.Add(MoveTemp(CompletedRecord));

	return;
}

void UEchoLoopSubsystem::ResetCharacterStatus(ACharacter* InCharacter)
{
	if (!IsValid(InCharacter)) return;

	InCharacter->StopJumping();

	// Reset Movement Component
	if (UCharacterMovementComponent* MovementComponent = InCharacter->GetCharacterMovement())
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->SetMovementMode(MOVE_Walking);
	}

	// Reset Transform & Reset Camera
	if (APlayerStart* pPlayerStart = this->PlayerStart.Get())
	{
		const FTransform& StartTransform = pPlayerStart->GetActorTransform();

		InCharacter->SetActorLocationAndRotation(
			StartTransform.GetLocation(),
			StartTransform.Rotator(),
			false, 
			nullptr,
			ETeleportType::TeleportPhysics
		);

		if (AController* Controller = InCharacter->GetController())
		{
			Controller->SetControlRotation(StartTransform.Rotator());
		}
	}

	return;
}

void UEchoLoopSubsystem::ResetActors()
{

	for (TWeakObjectPtr<ALoopResettableActor> ActorToReset : this->ActorsToReset)
	{
		ActorToReset.Get()->Reset();
	}

	return;
}

void UEchoLoopSubsystem::ResetEntityStatus()
{
	// Reset Status : Player
	AEchoLoopCharacter* PlayerCharacter = EchoLoopUtils::GetPlayerCharacter(this->GetWorld());
	this->ResetCharacterStatus(PlayerCharacter);

	if (UEchoRecordComponent* EchoRecordComponent = this->EchoRecorder.Get())
	{
		EchoRecordComponent->StartRecord();
	}



	// Reset Status : Echo
	for (TWeakObjectPtr<AEchoCharacter> EchoCharacter : this->EchoCharacterArray)
	{
		AEchoCharacter* Echo = EchoCharacter.Get();
		if (!IsValid(Echo)) continue;

		this->ResetCharacterStatus(Echo);

		UEchoReplayComponent* ReplayComponent = Echo->FindComponentByClass<UEchoReplayComponent>();
		if (!IsValid(ReplayComponent)) continue;

		ReplayComponent->StartReplay();
	}

	return;
}

void UEchoLoopSubsystem::SpawnEchoCharacter()
{

	AEchoCharacter* EchoCharacter = this->GetWorld()->SpawnActor<AEchoCharacter>(AEchoCharacter::StaticClass(), FTransform());
	
	check(EchoCharacter);

	// Initialize : Echo Replay Component
	if (UEchoReplayComponent* ReplayComponent = EchoCharacter->FindComponentByClass<UEchoReplayComponent>())
	{
		TSharedPtr<const FEchoRecord> EchoRecord = this->EchoRecordArray.Last();

		ReplayComponent->InitializeReplayComponent(EchoRecord);
	}


	AEchoLoopCharacter* PlayerCharacter = EchoLoopUtils::GetPlayerCharacter(this->GetWorld());
	EchoCharacter->CopyAppearanceFromCharacter(PlayerCharacter);

	this->EchoCharacterArray.Add(EchoCharacter);
	
	return;
}

void UEchoLoopSubsystem::StartLoopTimer()
{

	this->GetWorld()->GetTimerManager().SetTimer
	(
		this->LoopTimer,
		this,
		&UEchoLoopSubsystem::Callback_TimeLimitExceeded,
		EchoLoopConstants::LOOP_DURATION
	);

	return;
}
