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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vie")
	int32 ViesMeteroites;

	virtual void Tick(float DeltaTime) override;

	void InitMeteorite(FVector SpawnPos, FVector Direction);

	UFUNCTION()
	void BaisserVies();

protected:
	virtual void BeginPlay() override;

private:
	FVector MoveDirection;   
	float Speed;             
};
