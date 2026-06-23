
#include "EchoLoopUtils.h"
#include "EchoLoopCharacter.h"

AEchoLoopCharacter* EchoLoopUtils::GetPlayerCharacter(UWorld* InWorld)
{	
	APlayerController*	PlayerController	= InWorld			? InWorld->GetFirstPlayerController()	: nullptr;
	APawn*				PlayerPawn			= PlayerController	? PlayerController->GetPawn()			: nullptr;
	AEchoLoopCharacter* PlayerCharacter		= PlayerPawn		? Cast<AEchoLoopCharacter>(PlayerPawn)	: nullptr;

    return PlayerCharacter;
}
