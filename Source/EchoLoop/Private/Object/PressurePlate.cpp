#include "Object/PressurePlate.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "Interaction/Interactor.h"

#include "Materials/MaterialInterface.h"

APressurePlate::APressurePlate()
{
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
	Super::TurnOFF();

	check(this->GetMaterial_OFF());
	this->PlateMesh->SetMaterial(0, this->GetMaterial_OFF());

	return;
}

void APressurePlate::TurnON()
{
	Super::TurnON();

	check(this->GetMaterial_ON());
	this->PlateMesh->SetMaterial(0, this->GetMaterial_ON());

	return;
}

void APressurePlate::UpdateState()
{
	UE_LOG(LogTemp, Warning, TEXT("%d"), this->ActorsPressing.Num());

	bool bOldOn	= this->IsOn();
	bool bNewOn = (this->ActorsPressing.Num() > 0);

	if		(!bOldOn && bNewOn) { this->TurnON();  }
	else if (bOldOn && !bNewOn) { this->TurnOFF(); }

	return;
}