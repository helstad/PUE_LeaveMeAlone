// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

class USkeletalMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoEmptyDelegate);

USTRUCT(BlueprintType)
struct FAmmoWeapon
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Bullets = 30;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	int32 Clips = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	bool Infinite = true;
};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()

	/** PROPERTIES **/
public:
	ALMABaseWeapon();

	UPROPERTY(BlueprintAssignable)
	FOnAmmoEmptyDelegate OnAmmoEmpty;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Weapon")
	USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName SocketName = "Muzzle";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float TraceDistance = 800.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FAmmoWeapon AmmoWeapon{30, 0, true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	float Damage = 20.0f;
	
private:
	FAmmoWeapon CurrentAmmoWeapon;
	
	/** METHODS **/
public:
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void Fire();

	UFUNCTION(BlueprintCallable)
	void ChangeClip();
	bool IsClipFull() const;

	bool IsCurrentClipEmpty() const;
	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

	void MakeDamage(const FHitResult& Hit);
	
protected:
	virtual void BeginPlay() override;

	void Shoot();
	void DecrementBullets();
	
};