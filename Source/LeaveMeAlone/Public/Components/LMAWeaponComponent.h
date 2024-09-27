// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/LMABaseWeapon.h"
#include "LMAWeaponComponent.generated.h"

class ALMABaseWeapon;
class UAnimMontage;

UENUM(BlueprintType)
enum class EFireMode : uint8
{
	SingleShot UMETA(DisplayName = "Single Shot"),
	Automatic UMETA(DisplayName = "Automatic"),
};

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

	EFireMode CurrentFireMode = EFireMode::Automatic;
	
	/** METHODS **/
public:
	UFUNCTION()
	void StartFiring();

	UFUNCTION()
	void StopFiring();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void ToggleFireMode();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;
	
private:
	void Fire();
	void SpawnWeapon();
	void InitAnimNotify();

	UFUNCTION()
	void OnAmmoEmpty();
	
	void OnNotifyReloadFinished(USkeletalMeshComponent* SkeletalMesh);
	bool CanReload() const;
};