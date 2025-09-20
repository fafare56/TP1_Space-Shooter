// Fill out your copyright notice in the Description page of Project Settings.


#include "Vaisseau.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "Meteroite.h"
#include "Projectiles.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh du vaisseau comme racine
	// Mesh du vaisseau
	VaisseauMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaisseauMesh"));

	// Box de collision comme racine
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	// Attache le mesh au collision box
	VaisseauMesh->SetupAttachment(CollisionBox);

	// Paramètres de collision
	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f)); 
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_Pawn);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);

	// Mouvement
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;


	// Vies initiales
	Vies = 3;
}

// Called when the game starts or when spawned
void AVaisseau::BeginPlay()
{
	Super::BeginPlay();

	
	
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->Possess(this);
	}

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVaisseau::OnOverlapBegin);

}

// Called every frame
void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Pos = GetActorLocation();

	float MinX = -1500.f;
	float MaxX = -330.f;
	float MinY = -1420.f;
	float MaxY = 960.f;

	Pos.X = FMath::Clamp(Pos.X, MinX, MaxX);
	Pos.Y = FMath::Clamp(Pos.Y, MinY, MaxY);

	SetActorLocation(Pos);
}

// Bind des touches
void AVaisseau::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVaisseau::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVaisseau::MoveRight);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AVaisseau::Shoot);

}

void AVaisseau::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// Déplacement toujours selon l'axe Y du monde
		AddMovementInput(FVector::ForwardVector, Value);

		// Rotation du sprite
		if (Value > 0)
		{
			SetActorRotation(FRotator(0.f, -90.f, 0.f)); // Haut
		}
		else
		{
			SetActorRotation(FRotator(0.f, 90.f, 0.f)); // Bas
		}
	}
}

void AVaisseau::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// Déplacement toujours selon l'axe X du monde
		AddMovementInput(FVector::RightVector, Value);

		// Rotation du sprite
		if (Value > 0)
		{
			SetActorRotation(FRotator(0.f, 0.f, 0.f)); // Droite
		}
		else
		{
			SetActorRotation(FRotator(0.f, 180.f, 0.f)); // Gauche
		}
	}
}

void AVaisseau::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (OtherActor->IsA(AMeteroite::StaticClass())) 
		{
			OtherActor->Destroy();
			Vies--;

			if (Vies <= 0)
			{
				Destroy();
				FName NextLevel = FName("Menu_FIN");

				UGameplayStatics::OpenLevel(this, NextLevel);
			}
			
		}
	}
}

void AVaisseau::Shoot()
{
	if (ProjectileClass)
	{
		// Position du spawn du projectile (devant le vaisseau)
		FVector MuzzleLocation = GetActorLocation() + GetActorForwardVector() * 100.f;
		FRotator MuzzleRotation = GetActorRotation();

		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn du projectile
		GetWorld()->SpawnActor<AProjectiles>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
	}
}



