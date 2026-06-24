#include "Loop/LoopResettableActor.h"

#include "Loop/EchoLoopSubsystem.h"

ALoopResettableActor::ALoopResettableActor()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ALoopResettableActor::Reset()
{
	this->TurnOFF();

	return;
}

void ALoopResettableActor::BeginPlay()
{
	Super::BeginPlay();

	this->Reset();

	if (UEchoLoopSubsystem* LoopSystem = this->GetWorld()->GetSubsystem<UEchoLoopSubsystem>())
	{
		LoopSystem->RegisterResettableActor(this);
	}

	return;
}

void ALoopResettableActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{

	if (UEchoLoopSubsystem* LoopSystem = this->GetWorld()->GetSubsystem<UEchoLoopSubsystem>())
	{
		LoopSystem->UnregisterResettableActor(this);
	}

	Super::EndPlay(EndPlayReason);

	return;
}

void ALoopResettableActor::TurnOFF()
{
	this->bIsOn = false;

	return;
}

void ALoopResettableActor::TurnON()
{
	this->bIsOn = true;

	return;
}