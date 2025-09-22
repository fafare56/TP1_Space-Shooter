#include "Meteroite.h"
#include "MonGameInstance.h"
#include "Kismet/GameplayStatics.h"

AMeteroite::AMeteroite()
{
	PrimaryActorTick.bCanEverTick = true;
	Speed = 600.f; // vitesse
	ViesMeteroites = FMath::RandRange(1, 3);
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

void AMeteroite::BaisserVies()
{
	ViesMeteroites--;
	if (ViesMeteroites == 0)
	{
		Destroy();
		UMonGameInstance* GI = Cast<UMonGameInstance>(UGameplayStatics::GetGameInstance(this));
		if (GI)
		{
			GI->AddScore(10); // 10 points par météorite
		}
	}
		
}

