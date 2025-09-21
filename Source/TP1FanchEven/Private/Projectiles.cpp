#include "Projectiles.h"
#include "Meteroite.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "MonGameInstance.h"
#include "Kismet/GameplayStatics.h"

AProjectiles::AProjectiles()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->SetCollisionProfileName("OverlapAllDynamic");
	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 3.f;
}

void AProjectiles::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AProjectiles::OnOverlapBegin);
}

void AProjectiles::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AMeteroite::StaticClass()))
	{
		OtherActor->Destroy();
		Destroy();
		UMonGameInstance* GI = Cast<UMonGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (GI)
		{
			GI->AddScore(10); // 10 points par météorite
		}
	}
}

void AProjectiles::FireInDirection(const FVector& ShootDir)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDir * ProjectileMovement->InitialSpeed;
		SetActorRotation(ShootDir.Rotation());
	}
}
