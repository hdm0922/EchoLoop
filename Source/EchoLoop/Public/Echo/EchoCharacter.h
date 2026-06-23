#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EchoCharacter.generated.h"

UCLASS()
class ECHOLOOP_API AEchoCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	AEchoCharacter();

	virtual void Tick(float DeltaTime) override;

	void CopyAppearanceFromCharacter(const ACharacter* SourceCharacter);

	void DoMove(const struct FEchoRecordFrame& InEchoRecordFrame);

protected:

	virtual void BeginPlay() override;
	
};
