
#include "Echo/EchoRecordComponent.h"

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

	if (IsValid(this->EchoRecorder.Get()))
	{
		UE_LOG(LogTemp, Warning, TEXT("Valid Recorder Found."));
	}

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

	UE_LOG(LogTemp, Warning, TEXT("Saved Echo! At Slot %d"), this->EchoRecordArray.Num());

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
	// Get player
	ACharacter* PlayerCharacter = nullptr;
	{
		APlayerController*	PlayerController	= this->GetWorld()->GetFirstPlayerController();
		APawn*				PlayerPawn			= PlayerController ? PlayerController->GetPawn() : nullptr;
		PlayerCharacter							= PlayerPawn ? Cast<ACharacter>(PlayerPawn) : nullptr;
	}
	
	// Reset Status : Player
	this->ResetCharacterStatus(PlayerCharacter);

	// Reset Status : Echo
	UE_LOG(LogTemp, Warning, TEXT("Reset Echo Players ... "));

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
