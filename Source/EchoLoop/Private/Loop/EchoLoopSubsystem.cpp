
#include "Echo/EchoRecordComponent.h"
#include "Echo/EchoCharacter.h"
#include "EchoLoopCharacter.h"
#include "EchoLoopUtils.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerStart.h"

#include "Kismet/GameplayStatics.h"

#include "Loop/EchoLoopSubsystem.h"
#include "Loop/EchoLoopConstants.h"

#include "TimerManager.h"

void UEchoLoopSubsystem::RegisterEchoRecorder(UEchoRecordComponent* EchoRecordComponent)
{
	this->EchoRecorder = EchoRecordComponent;

	return;
}

void UEchoLoopSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);

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

void UEchoLoopSubsystem::ResetEntityStatus()
{
	// Reset Status : Player
	AEchoLoopCharacter* PlayerCharacter = EchoLoopUtils::GetPlayerCharacter(this->GetWorld());
	this->ResetCharacterStatus(PlayerCharacter);

	// Reset Status : Echo
	for (TWeakObjectPtr<AEchoCharacter> EchoCharacter : this->EchoCharacterArray)
	{
		this->ResetCharacterStatus(EchoCharacter.Get());
	}

	return;
}

void UEchoLoopSubsystem::SpawnEchoCharacter()
{

	AEchoCharacter* EchoCharacter = this->GetWorld()->SpawnActor<AEchoCharacter>(AEchoCharacter::StaticClass(), FTransform());
	
	if (IsValid(EchoCharacter))
	{
		AEchoLoopCharacter* PlayerCharacter = EchoLoopUtils::GetPlayerCharacter(this->GetWorld());
		EchoCharacter->CopyAppearanceFromCharacter(PlayerCharacter);

		this->EchoCharacterArray.Add(EchoCharacter);
	}
	
	return;
}

void UEchoLoopSubsystem::StartLoopTimer()
{

	if (UEchoRecordComponent* EchoRecordComponent = this->EchoRecorder.Get())
	{
		EchoRecordComponent->StartRecord();
	}

	this->GetWorld()->GetTimerManager().SetTimer
	(
		this->LoopTimer,
		this,
		&UEchoLoopSubsystem::Callback_TimeLimitExceeded,
		EchoLoopConstants::LOOP_DURATION
	);

	return;
}
