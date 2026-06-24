
#include "Echo/EchoCharacter.h"

#include "Components/CapsuleComponent.h"

#include "EchoLoopCharacter.h"
#include "Echo/EchoRecordTypes.h"
#include "Echo/EchoReplayComponent.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "Interaction/Interactable.h"

AEchoCharacter::AEchoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CreateDefaultSubobject<UEchoReplayComponent>(TEXT("EchoReplayComponent"));

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GetCharacterMovement()->RotationRate					= FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement		= true;
	GetCharacterMovement()->bUseControllerDesiredRotation	= false;
	GetCharacterMovement()->bRunPhysicsWithNoController		= true;



	static ConstructorHelpers::FObjectFinder<UMaterialInterface> EchoMaterialFinder(
		TEXT("/Game/EchoLoopMaterial/MAT_EchoCharacter.MAT_EchoCharacter")
	);

	if (EchoMaterialFinder.Succeeded())
	{
		this->EchoMaterial = EchoMaterialFinder.Object;
	}
}

void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	return;
}

void AEchoCharacter::InteractWith(IInteractable& InInteractable)
{
	UE_LOG(LogTemp, Warning, TEXT("Echo Interaction"));
	InInteractable.InteractWith(this);

	return;
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
		EchoMesh->SetMaterial(idx, this->EchoMaterial);
	}

	return;
}

void AEchoCharacter::DoMove(const FEchoRecordFrame& InEchoRecordFrame)
{
	const FRotator YawRotation		= FRotator(0.0, InEchoRecordFrame.ControlYaw, 0.0);

	const FVector ForwardDirection	= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection	= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	this->AddMovementInput(ForwardDirection,	InEchoRecordFrame.MoveY, true);
	this->AddMovementInput(RightDirection,		InEchoRecordFrame.MoveX, true);

	return;
}

void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	return;
}
