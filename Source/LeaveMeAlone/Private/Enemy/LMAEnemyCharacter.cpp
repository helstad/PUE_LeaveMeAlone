// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "Enemy/LMAEnemyCharacter.h"

#include "Components/LMAHealthComponent.h"

ALMAEnemyCharacter::ALMAEnemyCharacter()
{
 	PrimaryActorTick.bCanEverTick = true;
	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>(TEXT("Health Component"));
}

void ALMAEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALMAEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

