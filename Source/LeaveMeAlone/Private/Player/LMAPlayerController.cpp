// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "Player/LMAPlayerController.h"

ALMAPlayerController::ALMAPlayerController()
{
	
}

void ALMAPlayerController::BeginSpectatingState()
{
	SetControlRotation(FRotator(-75.0f, 0.0f, 0.0f));
	Super::BeginSpectatingState();
}

void ALMAPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	bShowMouseCursor = false;
}

