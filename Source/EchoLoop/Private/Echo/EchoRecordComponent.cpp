
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

void UEchoRecordComponent::StartRecord()
{
	this->AccumTime = 0.0f;
	this->EchoRecord = {};

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
	FEchoRecordFrame EchoRecordFrame;
	{
		AEchoLoopCharacter* PlayerCharacter = Cast<AEchoLoopCharacter>(this->GetOwner());
		check(PlayerCharacter);

		AController* PlayerController = PlayerCharacter->GetController();
		check(PlayerController);

		FVector2D MovementVector = PlayerCharacter->GetMovementVector2D();
		double ControlYaw = PlayerController->GetControlRotation().Yaw;

		EchoRecordFrame.MoveX		= MovementVector.X;
		EchoRecordFrame.MoveY		= MovementVector.Y;
		EchoRecordFrame.ControlYaw	= ControlYaw;
	}

	this->EchoRecord.FrameMovement.Add(EchoRecordFrame);

	return;
}
