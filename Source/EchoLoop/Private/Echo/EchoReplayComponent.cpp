
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

	while (this->AccumTime > EchoLoopConstants::FIXED_DELTA_TIME)
	{
		this->TickID++;
		this->AccumTime -= EchoLoopConstants::FIXED_DELTA_TIME;
	}

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

	return;
}

void UEchoReplayComponent::BeginPlay()
{
	Super::BeginPlay();

	return;
}

void UEchoReplayComponent::ReplayEchoFrame()
{
	int FrameMovementIdx = FMath::Clamp(static_cast<int>(this->TickID), 0, this->EchoRecord->FrameMovement.Num() - 1);
	FEchoRecordFrame EchoRecordFrame = this->EchoRecord->FrameMovement[FrameMovementIdx];

	AEchoCharacter* EchoCharacter = Cast<AEchoCharacter>(this->GetOwner());
	check(EchoCharacter);

	EchoCharacter->DoMove(EchoRecordFrame);
	
	return;
}