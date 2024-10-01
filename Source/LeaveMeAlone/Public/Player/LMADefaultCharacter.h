// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/LMAWeaponComponent.h"
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

	UFUNCTION(BlueprintCallable, Category = "Sprint")
	bool IsSprinting() const { return bIsSprinting; }

protected:
	/** Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	ULMAHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Components")
	ULMAWeaponComponent* WeaponComponent;

	/** Camera properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MinZoom = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float MaxZoom = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	float ZoomSpeed = 50.0f;

	/** Movement and rotation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RotationSpeed = 10.0f;

	/** Sprint properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float MaxStamina = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float Stamina = MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float SprintSpeedMultiplier = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaDrainRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaRecoveryRate = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaUpdateFrequency = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sprint")
	float StaminaBlockDuration = 3.0f;

	/** Animation */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* DeathMontage;

	/** Cursor properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	UMaterialInterface* CursorMaterial = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
	FVector CursorSize = FVector(20.0f, 32.0f, 32.0f);

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	FTimerHandle StaminaTimerHandle;
	FTimerHandle StaminaBlockTimerHandle;

	/** Character state */
	bool bCanRecoverStamina = true;
	bool bIsSprinting = false;
	bool bSprintBlocked = false;
	bool bCursorMoved;

	/** Camera properties */
	float YRotation = -75.0f;
	float ArmLength = 1400.0f;
	float FOV = 55.0f;

	/** Character controller reference */
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	/** Cursor decal component */
	UPROPERTY()
	UDecalComponent* CurrentCursor = nullptr;

	/** Initialization and input handling */
	void InitComponents();
	void MoveForward(float Value);
	void MoveRight(float Value);
	void ZoomCamera(const float AxisValue);
	void OnCursorMoved(float Value);

	/** Rotation and cursor update */
	void RotateTowardsCursor(float DeltaTime);
	void RotationPlayerOnCursor();
	void UpdateCursor();

	/** Sprint functionality */
	void StartSprinting();
	void StopSprinting();
	void DrainStamina();
	void RecoverStamina();
	void UnblockSprint();

	/** Health handling */
	UFUNCTION()
	void OnHealthChanged(float NewHealth);

	UFUNCTION()
	void OnDeath();
};