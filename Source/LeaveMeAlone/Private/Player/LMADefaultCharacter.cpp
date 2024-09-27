// LeaveMeAlone Game by Netology. All Rights Reserved.

#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "Components/LMAHealthComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

ALMADefaultCharacter::ALMADefaultCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	InitComponents();
}

void ALMADefaultCharacter::InitComponents()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->SetUsingAbsoluteRotation(true);
	SpringArmComponent->TargetArmLength = ArmLength;
	SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
	SpringArmComponent->bDoCollisionTest = false;
	SpringArmComponent->bEnableCameraLag = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->SetFieldOfView(FOV);
	CameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>(TEXT("HealthComponent"));
	WeaponComponent = CreateDefaultSubobject<ULMAWeaponComponent>(TEXT("WeaponComponent"));
}

void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);

	OnHealthChanged(HealthComponent->GetHealth());
	HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

	if (CursorMaterial)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(), CursorMaterial, CursorSize, FVector::ZeroVector);
	}

	UpdateCursor();
}

void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCursorMoved)
	{
		RotateTowardsCursor(DeltaTime);
		UpdateCursor();
		bCursorMoved = false;
	}

	if (!HealthComponent->IsDead())
	{
		RotationPlayerOnCursor();
	}
}

void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &ALMADefaultCharacter::ZoomCamera);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALMADefaultCharacter::StartSprinting);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALMADefaultCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, WeaponComponent, &ULMAWeaponComponent::StopFiring);
	PlayerInputComponent->BindAction("FireMode", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::ToggleFireMode);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, WeaponComponent, &ULMAWeaponComponent::Reload);
	
	PlayerInputComponent->BindAxis("MouseMove", this, &ALMADefaultCharacter::OnCursorMoved);
}

void ALMADefaultCharacter::RotateTowardsCursor(float DeltaTime)
{
	FHitResult ResultHit;
	if (PlayerController && PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit))
	{
		const FVector ActorLocation = GetActorLocation();
		const FVector TargetLocation = ResultHit.Location;
		const FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(ActorLocation, TargetLocation);
		const FRotator CurrentRotation = GetActorRotation();
		
		const FRotator NewRotation = FMath::RInterpTo(
			CurrentRotation,
			FRotator(0.0f, TargetRotation.Yaw, 0.0f),
			DeltaTime, RotationSpeed);

		SetActorRotation(NewRotation);
	}
}

void ALMADefaultCharacter::StartSprinting()
{
	if (Stamina > 0 && !bIsSprinting && !bSprintBlocked)
	{
		bIsSprinting = true;
		bCanRecoverStamina = false;
		
		GetWorld()->GetTimerManager().SetTimer(
			StaminaTimerHandle, this,
			&ALMADefaultCharacter::DrainStamina,
			StaminaUpdateFrequency, true);

		GetCharacterMovement()->MaxWalkSpeed *= SprintSpeedMultiplier;
	}
}

void ALMADefaultCharacter::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		bCanRecoverStamina = true;
		
		GetWorld()->GetTimerManager().SetTimer(
			StaminaTimerHandle, this,
			&ALMADefaultCharacter::RecoverStamina,
			StaminaUpdateFrequency, true);
		
		GetCharacterMovement()-> MaxWalkSpeed /= SprintSpeedMultiplier;
	}
}

void ALMADefaultCharacter::DrainStamina()
{
	if (Stamina > 0)
	{
		Stamina = FMath::Clamp(Stamina - StaminaDrainRate * StaminaUpdateFrequency, 0.0f, MaxStamina);
		if (Stamina <= 0.0f)
		{
			StopSprinting();
			bSprintBlocked = true;
			
			GetWorld()->GetTimerManager().SetTimer(
				StaminaBlockTimerHandle, this,
				&ALMADefaultCharacter::UnblockSprint,
				StaminaBlockDuration, false);
		}
	}
}

void ALMADefaultCharacter::RecoverStamina()
{
	if (Stamina < MaxStamina && bCanRecoverStamina)
	{
		Stamina = FMath::Clamp(Stamina + StaminaRecoveryRate * StaminaUpdateFrequency, 0.0f, MaxStamina);
		if (Stamina >= MaxStamina)
		{
			GetWorld()->GetTimerManager().ClearTimer(StaminaTimerHandle);
		}
	}
}

void ALMADefaultCharacter::UnblockSprint()
{
	bSprintBlocked = false;
	GetWorld()->GetTimerManager().ClearTimer(StaminaBlockTimerHandle);
}

void ALMADefaultCharacter::UpdateCursor()
{
	if (CurrentCursor && PlayerController)
	{
		FVector WorldLocation, WorldDirection;
		if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			FHitResult HitResult;
			FVector EndLocation = WorldLocation + WorldDirection * 10000.0f;
			GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, EndLocation, ECC_Visibility);

			if (HitResult.IsValidBlockingHit())
			{
				CurrentCursor->SetWorldLocation(HitResult.Location);
			}
		}
	}
}

void ALMADefaultCharacter::OnCursorMoved(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		bCursorMoved = true;
	}
}

void ALMADefaultCharacter::OnDeath()
{
	CurrentCursor->DestroyRenderState_Concurrent();

	PlayAnimMontage(DeathMontage);
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.0f);

	if (Controller)
	{
		Controller->ChangeState(NAME_Spectating);
	}
}

void ALMADefaultCharacter::RotationPlayerOnCursor()
{
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, HitResult);
		float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.Location).Yaw;
		SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));

		if (CurrentCursor)
		{
			CurrentCursor->SetWorldLocation(HitResult.Location);
		}
	}
}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth)
{
	GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Red,
		FString::Printf(TEXT("Health = %f"),
			NewHealth));
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		FVector ForwardVector = GetActorForwardVector();
		ForwardVector.Z = 0;
		ForwardVector.Normalize();

		AddMovementInput(ForwardVector, Value);
	}
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER)
	{
		FVector RightVector = GetActorRightVector();
		RightVector.Z = 0;
		RightVector.Normalize();

		AddMovementInput(RightVector, Value);
	}
}

void ALMADefaultCharacter::ZoomCamera(const float AxisValue)
{
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		SpringArmComponent->TargetArmLength = FMath::Clamp(
			SpringArmComponent->TargetArmLength - AxisValue * ZoomSpeed,
			MinZoom,
			MaxZoom);
	}
}