#include "Projectiles.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

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
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectiles::OnHit);
}

void AProjectiles::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
						 UPrimitiveComponent* OtherComp, FVector NormalImpulse,
						 const FHitResult& Hit)
{
	Destroy();
}

void AProjectiles::FireInDirection(const FVector& ShootDir)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = ShootDir * ProjectileMovement->InitialSpeed;
		// Optionnel : corrige la rotation si le mesh n’est pas orienté correctement
		SetActorRotation(ShootDir.Rotation());
	}
}
