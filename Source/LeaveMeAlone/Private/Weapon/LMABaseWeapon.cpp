// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "Weapon/LMABaseWeapon.h"

#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeapon, All, All);

ALMABaseWeapon::ALMABaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));
	SetRootComponent(MeshComponent);
}

void ALMABaseWeapon::MakeDamage(const FHitResult& Hit)
{
	const auto Zombie = Hit.GetActor();
	if (!Zombie) return;

	const auto Pawn = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!Pawn) return;

	// const auto Controller = Pawn->GetController<APlayerController>();
	// if (!Controller) return;

	Zombie->TakeDamage(Damage, FDamageEvent(), Pawn, this);
}

void ALMABaseWeapon::BeginPlay()
{
	Super::BeginPlay();

	CurrentAmmoWeapon = AmmoWeapon;
}

void ALMABaseWeapon::Shoot()
{
	const FTransform SocketTransform = MeshComponent->GetSocketTransform(SocketName);
	const FVector TraceStart = SocketTransform.GetLocation();
	const FVector ShootDirection = SocketTransform.GetRotation().GetForwardVector();
	const FVector TraceEnd = TraceStart + (ShootDirection * TraceDistance);
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Black, false, 1.0f, 0, 2.0f);

	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility);

	FVector TracerEnd = TraceEnd;
	
	if (HitResult.bBlockingHit)
	{
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5.0f, 24, FColor::Red, false, 1.0f);
		MakeDamage(HitResult);
		TracerEnd = HitResult.ImpactPoint;
	}

	DecrementBullets();
}

void ALMABaseWeapon::DecrementBullets()
{
	CurrentAmmoWeapon.Bullets--;
	if (IsCurrentClipEmpty())
	{
		OnAmmoEmpty.Broadcast();
	}
}

bool ALMABaseWeapon::IsCurrentClipEmpty() const
{
	return CurrentAmmoWeapon.Bullets == 0;
}

void ALMABaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ALMABaseWeapon::Fire()
{
	Shoot();
}

void ALMABaseWeapon::ChangeClip()
{
	CurrentAmmoWeapon.Bullets = AmmoWeapon.Bullets;
}

bool ALMABaseWeapon::IsClipFull() const
{
	return CurrentAmmoWeapon.Bullets >= AmmoWeapon.Bullets;
}

