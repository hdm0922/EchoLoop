#include "Interaction/Lever.h"

#include "Components/StaticMeshComponent.h"

#include "Materials/MaterialInterface.h"

ALever::ALever()
{
	this->LeverMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeverMesh"));
	this->LeverMesh->SetCollisionProfileName(TEXT("BlockAll"));

	SetRootComponent(this->LeverMesh);
}

void ALever::InteractWith(AActor* InActor)
{
	if (this->IsOn())	{ this->TurnOFF(); }
	else				{ this->TurnON();  }

	return;
}

void ALever::BeginPlay()
{
	Super::BeginPlay();
	
	return;
}

void ALever::TurnOFF()
{
	Super::TurnOFF();

	check(this->GetMaterial_OFF());
	this->LeverMesh->SetMaterial(0, this->GetMaterial_OFF());

	return;
}

void ALever::TurnON()
{
	Super::TurnON();

	check(this->GetMaterial_ON());
	this->LeverMesh->SetMaterial(0, this->GetMaterial_ON());

	return;
}
