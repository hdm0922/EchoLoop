#include "Interaction/Lever.h"

#include "Components/StaticMeshComponent.h"

#include "Materials/MaterialInterface.h"

ALever::ALever()
{
	PrimaryActorTick.bCanEverTick = false;

	this->LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	this->LeverMesh->SetCollisionProfileName(TEXT("BlockAll"));

	SetRootComponent(this->LeverMesh);
}

void ALever::InteractWith(AActor* InActor)
{
	if (this->bIsOn) { this->TurnOFF(); }
	else { this->TurnON(); }

	return;
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	
	this->TurnOFF();

	return;
}

void ALever::TurnOFF()
{
	check(this->OffMaterial);

	this->bIsOn = false;
	this->LeverMesh->SetMaterial(0, this->OffMaterial);

	return;
}

void ALever::TurnON()
{
	check(this->OnMaterial);

	this->bIsOn = true;
	this->LeverMesh->SetMaterial(0, this->OnMaterial);

	return;
}
