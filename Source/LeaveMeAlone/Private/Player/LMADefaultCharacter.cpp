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

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

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
}

void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
	PlayerInputComponent->BindAxis("ZoomCamera", this, &ALMADefaultCharacter::ZoomCamera);

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
		const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, FRotator(0.0f, TargetRotation.Yaw, 0.0f), DeltaTime, RotationSpeed);

		SetActorRotation(NewRotation);
	}
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

void ALMADefaultCharacter::MoveForward(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER && CameraComponent)
	{
		FVector ForwardVector = CameraComponent->GetForwardVector();
		ForwardVector.Z = 0;
		ForwardVector.Normalize();

		AddMovementInput(ForwardVector, Value);
	}
}

void ALMADefaultCharacter::MoveRight(float Value)
{
	if (FMath::Abs(Value) > KINDA_SMALL_NUMBER && CameraComponent)
	{
		FVector RightVector = CameraComponent->GetRightVector();
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