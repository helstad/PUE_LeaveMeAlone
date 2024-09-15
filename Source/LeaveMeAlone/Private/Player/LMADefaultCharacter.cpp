// LeaveMeAlone Game by Netology. All Rights Reserved.

#include "Player/LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
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
}

void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateCursor();
}

void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RotateTowardsCursor(DeltaTime);
	UpdateCursor();
}

void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &ALMADefaultCharacter::ZoomCamera);
}

void ALMADefaultCharacter::RotateTowardsCursor(float DeltaTime)
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		FHitResult ResultHit;
		PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

		if (ResultHit.bBlockingHit)
		{
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
				GetActorLocation(), ResultHit.Location);
			FRotator CurrentRotation = GetActorRotation();

			FRotator NewRotation = FMath::RInterpTo(
				CurrentRotation,
				FRotator(0.0f, TargetRotation.Yaw, 0.0f), 
				DeltaTime,
				RotationSpeed);
			SetActorRotation(NewRotation);
		}
	}
}


void ALMADefaultCharacter::UpdateCursor()
{
	if (CursorMaterial && !CurrentCursor)
	{
		CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(), CursorMaterial, CursorSize, FVector(0));
	}

	if (CurrentCursor)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			FHitResult ResultHit;
			PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);

			if (ResultHit.bBlockingHit)
			{
				CurrentCursor->SetWorldLocation(ResultHit.Location);
			}
		}
	}
}

void ALMADefaultCharacter::MoveForward(float Value)
{
	FVector ForwardVector = CameraComponent->GetForwardVector();
	ForwardVector.Z = 0;
	ForwardVector.Normalize();
	
	AddMovementInput(ForwardVector, Value);
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	FVector RightVector = CameraComponent->GetRightVector();
	RightVector.Z = 0;
	RightVector.Normalize();
	
	AddMovementInput(RightVector, Value);
}

void ALMADefaultCharacter::ZoomCamera(const float AxisValue)
{
	if (FMath::Abs(AxisValue) > KINDA_SMALL_NUMBER)
	{
		float NewLength = SpringArmComponent->TargetArmLength - AxisValue * ZoomSpeed;
		NewLength = FMath::Clamp(NewLength, MinZoom, MaxZoom);
		SpringArmComponent->TargetArmLength = NewLength;
	}
}