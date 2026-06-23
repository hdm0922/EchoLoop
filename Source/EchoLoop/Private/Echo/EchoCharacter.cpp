
#include "Echo/EchoCharacter.h"
#include "EchoLoopCharacter.h"

AEchoCharacter::AEchoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEchoCharacter::CopyAppearanceFromCharacter(const ACharacter* SourceCharacter)
{
	check(SourceCharacter);

	USkeletalMeshComponent* SourceMesh	= SourceCharacter->GetMesh();
	USkeletalMeshComponent* EchoMesh	= this->GetMesh();

	check(SourceMesh);
	check(EchoMesh);

	EchoMesh->SetSkeletalMeshAsset(SourceMesh->GetSkeletalMeshAsset());
	EchoMesh->SetAnimInstanceClass(SourceMesh->GetAnimClass());
	EchoMesh->SetRelativeTransform(SourceMesh->GetRelativeTransform());

	for (int32 idx = 0; idx < SourceMesh->GetNumMaterials(); idx++)
	{
		EchoMesh->SetMaterial(idx, SourceMesh->GetMaterial(idx));
	}

	return;
}

void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

