
#include "Echo/EchoRecordComponent.h"

#include "Loop/EchoLoopConstants.h"

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

void UEchoRecordComponent::BeginPlay()
{
	Super::BeginPlay();
	
	this->ResetRecord();

	return;
}

void UEchoRecordComponent::RecordFixedTick()
{

	return;
}

void UEchoRecordComponent::ResetRecord()
{

	this->AccumTime = 0.0f;

	return;
}
