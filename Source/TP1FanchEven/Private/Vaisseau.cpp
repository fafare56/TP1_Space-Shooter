// Fill out your copyright notice in the Description page of Project Settings.


#include "Vaisseau.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh du vaisseau comme racine
	VaisseauMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaisseauMesh"));
	RootComponent = VaisseauMesh;

	// Composant de mouvement (permet AddMovementInput sur un Pawn)
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;
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


