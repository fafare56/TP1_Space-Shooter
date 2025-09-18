#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Meteroite.generated.h"

UCLASS()
class TP1FANCHEVEN_API AMeteroite : public AActor
{
	GENERATED_BODY()
    
public:
	AMeteroite();

	virtual void Tick(float DeltaTime) override;

	// Initialisation de la météorite
	void InitMeteorite(FVector SpawnPos, FVector Direction);

protected:
	virtual void BeginPlay() override;

private:
	FVector MoveDirection;   // Direction normalisée
	float Speed;             // Vitesse en unités/s
};
