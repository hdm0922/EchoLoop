
#include "Echo/EchoReplayComponent.h"

#include "Character/EchoCharacter.h"

#include "Echo/EchoRecordTypes.h"

#include "Loop/EchoLoopConstants.h"

#include "Interaction/Interactable.h"

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
	this->AccumTime			= 0.0f;
	this->TickID			= 0u;
	this->JumpIdx			= 0u;
	this->InteractionIdx	= 0u;

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

	this->ReplayEchoInteraction(*EchoCharacter);
	this->ReplayEchoJump(*EchoCharacter);
	this->ReplayEchoMovement(*EchoCharacter);

	return;
}

void UEchoReplayComponent::ReplayEchoInteraction(AEchoCharacter& InEchoCharacter)
{
	const TArray<FEchoInteractionCommand>& CommandBuffer = this->EchoRecord->InteractionCommand;

	if (!CommandBuffer.IsValidIndex(this->InteractionIdx))			return;
	if (this->TickID < CommandBuffer[this->InteractionIdx].TickID)	return;

	AActor* InteractableActor = CommandBuffer[this->InteractionIdx].InteractableActor.Get();
	InEchoCharacter.InteractWith(*Cast<IInteractable>(InteractableActor));

	this->InteractionIdx++;

	return;
}

void UEchoReplayComponent::ReplayEchoJump(AEchoCharacter& InEchoCharacter)
{
	if (this->bJumpCommandLastFrame)
	{
		InEchoCharacter.StopJumping();

		this->bJumpCommandLastFrame = false;
	}



	const TArray<uint32>& CommandBuffer = this->EchoRecord->JumpCommand;

	if (!CommandBuffer.IsValidIndex(this->JumpIdx))		return;
	if (CommandBuffer[this->JumpIdx] > this->TickID)	return;

	InEchoCharacter.Jump();

	this->JumpIdx++;
	this->bJumpCommandLastFrame = true;

	return;
}

void UEchoReplayComponent::ReplayEchoMovement(AEchoCharacter& InEchoCharacter)
{
	int idx = FMath::Clamp(static_cast<int>(this->TickID), 0, EchoLoopConstants::TOTAL_TICKS - 1);
	FEchoRecordFrame EchoRecordFrame = this->EchoRecord->FrameMovement[idx];

	InEchoCharacter.DoMove(EchoRecordFrame);

	return;
}