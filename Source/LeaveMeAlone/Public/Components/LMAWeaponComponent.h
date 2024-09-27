// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LEAVEMEALONE_API ULMAWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
	/** PROPERTIES **/
public:
	ULMAWeaponComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon");
	float FireRate = 0.1f;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	TSubclassOf<ALMABaseWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* ReloadMontage;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Weapon")
	FName SocketName = "r_Weapon_Socket";

private:
	UPROPERTY()
	ALMABaseWeapon* Weapon = nullptr;

	FTimerHandle FireTimerHandle;
	
	bool AnimReloading = false;
	
	/** METHODS **/
public:
	// void Fire();

	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	UFUNCTION(BlueprintCallable)
	void Reload();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
private:
	void FireRepeatedly();
	void SpawnWeapon();
	void InitAnimNotify();

	UFUNCTION()
	void OnAmmoEmpty();
	
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool CanReload() const;
};