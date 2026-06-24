
#include "Echo/EchoRecordComponent.h"
#include "EchoLoopCharacter.h"

#include "Loop/EchoLoopConstants.h"
#include "Loop/EchoLoopSubsystem.h"


UEchoRecordComponent::UEchoRecordComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEchoRecordComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->AccumTime += DeltaTime;

	while (this->AccumTime > EchoLoopConstants::FIXED_DELTA_TIME)
	{
		this->RecordFixedTick();

		this->AccumTime -= EchoLoopConstants::FIXED_DELTA_TIME;
	}

	return;
}

TSharedPtr<const struct FEchoRecord> UEchoRecordComponent::FinishRecord()
{
	return MakeShared<FEchoRecord>(MoveTemp(this->EchoRecord));
}

void UEchoRecordComponent::RecordInteractionCommand(TWeakObjectPtr<AActor> InActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Recording Interaction!"));

	FEchoInteractionCommand EchoInteractionCommand(this->TickID, InActor);
	this->EchoRecord.InteractionCommand.Add(EchoInteractionCommand);

	return;
}

void UEchoRecordComponent::RecordJumpCommand()
{
	UE_LOG(LogTemp, Warning, TEXT("Recording Jump!"));
	this->EchoRecord.JumpCommand.Add(this->TickID);

	return;
}

void UEchoRecordComponent::StartRecord()
{
	this->AccumTime = 0.0f;
	this->TickID	= 0u;

	this->EchoRecord = {};
	this->EchoRecord.FrameMovement.SetNum(EchoLoopConstants::TOTAL_TICKS);
	this->EchoRecord.JumpCommand.Empty();
	this->EchoRecord.InteractionCommand.Empty();

	return;
}

void UEchoRecordComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (UWorld* World = this->GetWorld())
	{
		UEchoLoopSubsystem* EchoLoopSubsystem = World->GetSubsystem<UEchoLoopSubsystem>();
		EchoLoopSubsystem->RegisterEchoRecorder(this);
	}

	this->StartRecord();

	return;
}

void UEchoRecordComponent::RecordFixedTick()
{
	AEchoLoopCharacter* PlayerCharacter = Cast<AEchoLoopCharacter>(this->GetOwner());
	check(PlayerCharacter);

	AController* PlayerController = PlayerCharacter->GetController();
	check(PlayerController);



	FEchoRecordFrame EchoRecordFrame;
	{
		FVector2D	MovementVector	= PlayerCharacter->GetMovementVector2D();
		double		ControlYaw		= PlayerController->GetControlRotation().Yaw;

		EchoRecordFrame.MoveX		= MovementVector.X;
		EchoRecordFrame.MoveY		= MovementVector.Y;
		EchoRecordFrame.ControlYaw	= ControlYaw;
	}

	int idx = FMath::Clamp(static_cast<int>(this->TickID), 0, EchoLoopConstants::TOTAL_TICKS - 1);
	this->EchoRecord.FrameMovement[idx] = EchoRecordFrame;



	this->TickID++;

	return;
}
