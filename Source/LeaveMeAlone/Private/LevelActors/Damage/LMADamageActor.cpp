// LeaveMeAlone Game by Netology. All Rights Reserved.


#include "LevelActors/Damage/LMADamageActor.h"

#include "Kismet/GameplayStatics.h"

ALMADamageActor::ALMADamageActor()
{
 	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetSphereRadius(SphereRadius);
	SetRootComponent(SphereComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(SphereComponent);
	StaticMeshComponent->SetRelativeScale3D(FVector(2.0f, 2.0f, 0.05f));
	StaticMeshComponent->SetRelativeLocation(FVector(0.0f, 0.0f, -4.0f));

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(SphereComponent);
	NiagaraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
}

void ALMADamageActor::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraSystem)
	{
		NiagaraComponent->SetAsset(NiagaraSystem);
		NiagaraComponent->Activate(true);
	}
	
}

void ALMADamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UGameplayStatics::ApplyRadialDamage(
		GetWorld(),
		Damage,
		GetActorLocation(),
		SphereRadius,
		nullptr,
		{},
		this,
		nullptr,
		false);
}

