#include "Interaction/PressurePlate.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Interaction/Interactor.h"

#include "Materials/MaterialInterface.h"

APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = false;

	this->TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	this->TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	this->TriggerBox->SetGenerateOverlapEvents(true);
	this->TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	this->TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	SetRootComponent(this->TriggerBox);

	this->PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlateMesh"));
	this->PlateMesh->SetupAttachment(TriggerBox);
}

void APressurePlate::BeginPlay()
{
	Super::BeginPlay();
	
	this->TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APressurePlate::OnOverlapBegin);
	this->TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APressurePlate::OnOverlapEnd);

	this->TurnOFF();

	return;
}

void APressurePlate::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UInteractor>()) return;

	this->ActorsPressing.Add(OtherActor);

	this->UpdateState();

	return;
}

void APressurePlate::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!this->ActorsPressing.Contains(OtherActor)) return;

	this->ActorsPressing.Remove(OtherActor);

	this->UpdateState();

	return;
}

void APressurePlate::TurnOFF()
{
	check(this->OffMaterial);

	this->bIsOn = false;
	this->PlateMesh->SetMaterial(0, this->OffMaterial);

	return;
}

void APressurePlate::TurnON()
{
	check(this->OnMaterial);

	this->bIsOn = true;
	this->PlateMesh->SetMaterial(0, this->OnMaterial);

	return;
}

void APressurePlate::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), this->ActorsPressing.Num());

	bool bNewOn = (this->ActorsPressing.Num() > 0);

	if		(!this->bIsOn && bNewOn) { this->TurnON();  }
	else if (this->bIsOn && !bNewOn) { this->TurnOFF(); }

	return;
}