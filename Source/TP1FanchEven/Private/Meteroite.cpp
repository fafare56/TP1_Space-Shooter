#include "Meteroite.h"

AMeteroite::AMeteroite()
{
	PrimaryActorTick.bCanEverTick = true;
	Speed = 600.f; // vitesse
}

void AMeteroite::BeginPlay()
{
	Super::BeginPlay();
}

void AMeteroite::InitMeteorite(FVector SpawnPos, FVector Direction)
{
	SpawnPos.Z = 0.f;            // reste dans le plan XY
	SetActorLocation(SpawnPos);
	MoveDirection = Direction.GetSafeNormal();
	MoveDirection.Z = 0.f;       // s'assurer qu'on reste en XY
}

void AMeteroite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!MoveDirection.IsZero())	
	{
		FVector NewLocation = GetActorLocation() + MoveDirection * Speed * DeltaTime;
		NewLocation.Z = 0.f;   // verrouiller Z
		SetActorLocation(NewLocation);
	}
}
