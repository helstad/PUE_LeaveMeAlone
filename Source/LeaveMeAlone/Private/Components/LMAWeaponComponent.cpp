// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "Components/LMAWeaponComponent.h"

#include "Animations/LMAReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"

ULMAWeaponComponent::ULMAWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
	InitAnimNotify();
}

void ULMAWeaponComponent::FireRepeatedly()
{
	if (Weapon && !AnimReloading)
	{
		if (Weapon->IsCurrentClipEmpty())
		{
			OnAmmoEmpty();
			return;
		}
		Weapon->Fire();
	}
}

void ULMAWeaponComponent::StartFiring()
{
	if (Weapon && !AnimReloading)
	{
		GetWorld()->GetTimerManager().SetTimer(
			FireTimerHandle, this,
			&ULMAWeaponComponent::FireRepeatedly,
			FireRate, true);
	}
}

void ULMAWeaponComponent::StopFiring()
{
	GetWorld()->GetTimerManager().ClearTimer(FireTimerHandle);
}

void ULMAWeaponComponent::Reload()
{
	if (!CanReload())
		return;

	AnimReloading = true;
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	Character->PlayAnimMontage(ReloadMontage);
}

void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void ULMAWeaponComponent::SpawnWeapon()
{
	Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);

	if (Weapon)
	{
		Weapon->OnAmmoEmpty.AddDynamic(this, &ULMAWeaponComponent::OnAmmoEmpty);
		
		if (const auto Character = Cast<ACharacter>(GetOwner()))
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, SocketName);
		}
	}
}

void ULMAWeaponComponent::InitAnimNotify()
{
	if (!ReloadMontage)
		return;

	const auto NotifiesEvents = ReloadMontage->Notifies;
	for (auto NotifyEvent : NotifiesEvents)
	{
		if (auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify))
		{
			ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
			break;
		}
	}
}

void ULMAWeaponComponent::OnAmmoEmpty()
{
	Reload();
}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh)
{
	const auto Character = Cast<ACharacter>(GetOwner());
	if (Character->GetMesh() == SkeletalMesh)
	{
		AnimReloading = false;
		Weapon->ChangeClip();
	}
}

bool ULMAWeaponComponent::CanReload() const
{
	return !AnimReloading && !Weapon->IsClipFull();
}