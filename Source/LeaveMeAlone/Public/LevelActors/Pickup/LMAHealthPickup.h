// LeaveMeAlone Game by Netology. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Player/LMADefaultCharacter.h"
#include "LMAHealthPickup.generated.h"

UCLASS()
class LEAVEMEALONE_API ALMAHealthPickup : public AActor
{
	GENERATED_BODY()

public:
	ALMAHealthPickup();

protected:
	UPROPERTY(VisibleAnywhere, Category="Pickup")
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
	float RespawnTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup", meta = (ClampMin=5.0f, ClampMax=100.0f))
	float HealthFromPickup = 100.0f;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void BeginPlay() override;

public:
	bool GivePickup(ALMADefaultCharacter* Character);

	void PickupWasTaken();

	void RespawnPickup();
	
	virtual void Tick(float DeltaTime) override;

};