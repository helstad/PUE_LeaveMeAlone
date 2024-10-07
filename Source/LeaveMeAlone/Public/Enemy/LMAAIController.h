// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "LMAAIController.generated.h"

UCLASS()
class LEAVEMEALONE_API ALMAAIController : public AAIController
{
	GENERATED_BODY()

	/* PROPERTIES */
public:

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunningSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkingSpeed = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	FName IsRunningKey = "bIsRunning";

	/* METHODS */
public:
protected:
	void UpdateMovementSpeed();
	
	virtual void Tick(float DeltaSeconds) override;
	virtual void OnPossess(APawn* InPawn) override;
private:
};
