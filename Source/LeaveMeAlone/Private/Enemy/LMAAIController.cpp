// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "Enemy/LMAAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Enemy/LMAEnemyCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void ALMAAIController::UpdateMovementSpeed()
{
	bool bIsRunning = false;
	if (UBlackboardComponent* BlackboardComponent = GetBlackboardComponent())
	{
		bIsRunning = BlackboardComponent->GetValueAsBool(IsRunningKey);
	}

	ACharacter* ControlledCharacter = Cast<ACharacter>(GetPawn());
	if (ControlledCharacter && ControlledCharacter->GetCharacterMovement())
	{
		if (bIsRunning)
		{
			ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
		}
		else
		{
			ControlledCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkingSpeed;
		}
	}
}

void ALMAAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateMovementSpeed();
}

void ALMAAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (const auto AICharacter = Cast<ALMAEnemyCharacter>(InPawn))
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}