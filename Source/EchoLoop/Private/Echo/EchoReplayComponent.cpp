
#include "Echo/EchoReplayComponent.h"

#include "Echo/EchoCharacter.h"
#include "Echo/EchoRecordTypes.h"

#include "Loop/EchoLoopConstants.h"

UEchoReplayComponent::UEchoReplayComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UEchoReplayComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	this->AccumTime += DeltaTime;

	int FixedTickCount = static_cast<int>(this->AccumTime / EchoLoopConstants::FIXED_DELTA_TIME);
	
	this->TickID	+= FixedTickCount;
	this->AccumTime -= static_cast<float>(FixedTickCount) * EchoLoopConstants::FIXED_DELTA_TIME;

	this->ReplayEchoFrame();

	return;
}

void UEchoReplayComponent::InitializeReplayComponent(TSharedPtr<const struct FEchoRecord> InEchoRecord)
{
	this->EchoRecord = MoveTemp(InEchoRecord);

	return;
}

void UEchoReplayComponent::StartReplay()
{
	this->AccumTime = 0.0f;
	this->TickID	= 0u;
	this->JumpIdx	= 0u;

	this->bJumpCommandLastFrame = false;

	return;
}

void UEchoReplayComponent::BeginPlay()
{
	Super::BeginPlay();

	return;
}

void UEchoReplayComponent::ReplayEchoFrame()
{
	AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(this->GetOwner());
	check(EchoCharacter);



	int idx = FMath::Clamp(static_cast<int>(this->TickID), 0, EchoLoopConstants::TOTAL_TICKS - 1);
	FEchoRecordFrame EchoRecordFrame = this->EchoRecord->FrameMovement[idx];

	EchoCharacter->DoMove(EchoRecordFrame);
	


	if (this->bJumpCommandLastFrame)
	{
		EchoCharacter->StopJumping();

		this->bJumpCommandLastFrame = false;
	}

	if (
		this->EchoRecord->JumpCommand.IsValidIndex(this->JumpIdx) &&
		this->EchoRecord->JumpCommand[this->JumpIdx] <= this->TickID)
	{
		EchoCharacter->Jump();

		this->JumpIdx++;
		this->bJumpCommandLastFrame = true;
	}

	return;
}