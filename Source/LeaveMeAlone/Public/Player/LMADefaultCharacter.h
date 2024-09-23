// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ALMADefaultCharacter();

	UFUNCTION()
	ULMAHealthComponent* GetHealthComponent() const { return HealthComponent; }
	
	UFUNCTION(BlueprintCallable, Category="Sprint")
	bool IsSprinting() const { return bIsSprinting; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 32.0f, 32.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinZoom = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoom = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 10.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	float Stamina = MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	float SpringSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	float StaminaDrainRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	float StaminaRecoveryRate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spring")
	float StaminaUpdateFrequency = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Sprint")
	float StaminaBlockDuration = 3.0f;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle StaminaTimerHandle;
	FTimerHandle StaminaBlockTimerHandle;
	
	bool bCanRecoverStamina = true;
	bool bIsSprinting = false;
	bool bSprintBlocked = false;
	bool bCursorMoved;
	
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	void InitComponents();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void ZoomCamera(const float AxisValue);
	void RotateTowardsCursor(float DeltaTime);

	void StartSprinting();
	void StopSprinting();
	void DrainStamina();
	void RecoverStamina();
	void UnblockSprint();

	void UpdateCursor();
	void OnCursorMoved(float Value);

	void OnDeath();
	void RotationPlayerOnCursor();

	void OnHealthChanged(float NewHealth);
};