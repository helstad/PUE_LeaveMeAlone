// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "Components/LMAHealthComponent.h"

ULMAHealthComponent::ULMAHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void ULMAHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	OnHealthChanged.Broadcast(Health);
	
	AActor* OwnerComponent = GetOwner();

	if (OwnerComponent)
	{
		OwnerComponent->OnTakeAnyDamage.AddDynamic(this, &ULMAHealthComponent::OnTakeAnyDamage);
	}
}

void ULMAHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.0f || IsDead()) return;

	ApplyDamage(Damage);
}

void ULMAHealthComponent::ApplyDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

bool ULMAHealthComponent::AddHealth(float NewHealth)
{
	if (NewHealth <= 0.0f || IsDead() || IsHealthFull()) return false;

	Health = FMath::Clamp(Health + NewHealth, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(Health);
	
	return true;
}