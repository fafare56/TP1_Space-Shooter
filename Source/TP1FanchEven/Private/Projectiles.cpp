#include "Projectiles.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

AProjectiles::AProjectiles()
{
	PrimaryActorTick.bCanEverTick = true;

	// Collision
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.f);
	CollisionComp->SetCollisionProfileName("BlockAllDynamic");
	RootComponent = CollisionComp;

	// Mesh optionnel
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);

	// Mouvement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Durée de vie auto
	InitialLifeSpan = 3.0f;
}

void AProjectiles::BeginPlay()
{
	Super::BeginPlay();
	CollisionComp->OnComponentHit.AddDynamic(this, &AProjectiles::OnHit);
}

void AProjectiles::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AProjectiles::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
								UPrimitiveComponent* OtherComp, FVector NormalImpulse,
								const FHitResult& Hit)
{
	if (OtherActor && OtherActor != this)
	{
		// Exemple : détruire projectile au contact
		Destroy();
	}
}
