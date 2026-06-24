#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interaction/Interactor.h"
#include "EchoCharacter.generated.h"

class IInteractable;

struct FEchoRecordFrame;

UCLASS()
class ECHOLOOP_API AEchoCharacter : public ACharacter, public IInteractor
{
	GENERATED_BODY()

public:

	AEchoCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void InteractWith(IInteractable& InInteractable) override;

	void CopyAppearanceFromCharacter(const ACharacter* SourceCharacter);

	void DoMove(const FEchoRecordFrame& InEchoRecordFrame);

	

protected:

	virtual void BeginPlay() override;
		
private:

	TObjectPtr<UMaterialInterface> EchoMaterial;
};
