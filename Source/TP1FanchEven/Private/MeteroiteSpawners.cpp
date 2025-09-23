#include "MeteroiteSpawners.h"
#include "Meteroite.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

AMeteroiteSpawners::AMeteroiteSpawners()
{
    PrimaryActorTick.bCanEverTick = false;

    SpawnInterval = 2.0f;
    SpawnDistance = 1200.0f;
    ScreenHalfWidth = 800.0f;
    ScreenHalfHeight = 600.0f;
}

void AMeteroiteSpawners::BeginPlay()
{
    Super::BeginPlay();

    if (MeteroiteClass)
    {
        GetWorld()->GetTimerManager().SetTimer(
            SpawnTimerHandle,
            this,
            &AMeteroiteSpawners::SpawnMeteorite,
            SpawnInterval,
            true
        );
    }
}

void AMeteroiteSpawners::SpawnMeteorite()
{
    if (!MeteroiteClass) return;

    AActor* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    FVector PlayerPos = PlayerPawn->GetActorLocation();

    float MinX = -1500.f;
    float MaxX = -330.f;
    float MinY = -1420.f;
    float MaxY = 960.f;

    FVector SpawnPos = PlayerPos;

    int Side = FMath::RandRange(0, 3); 

    switch (Side)
    {
    case 0: 
        SpawnPos.X = MinX - 200.f; 
        SpawnPos.Y = FMath::RandRange(MinY, MaxY);
        break;
    case 1: 
        SpawnPos.X = MaxX + 200.f;
        SpawnPos.Y = FMath::RandRange(MinY, MaxY);
        break;
    case 2:
        SpawnPos.Y = MinY - 200.f;
        SpawnPos.X = FMath::RandRange(MinX, MaxX);
        break;
    case 3:
        SpawnPos.Y = MaxY + 200.f;
        SpawnPos.X = FMath::RandRange(MinX, MaxX);
        break;
    }

    SpawnPos.Z = 0.f; 

    FActorSpawnParameters SpawnParams;
    AMeteroite* Meteor = GetWorld()->SpawnActor<AMeteroite>(
        MeteroiteClass,
        SpawnPos,
        FRotator::ZeroRotator,
        SpawnParams
    );

    if (Meteor)
    {
        FVector Dir = (PlayerPos - SpawnPos).GetSafeNormal();
        Meteor->InitMeteorite(SpawnPos, Dir);
    }
}

