#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectiles.generated.h"

UCLASS()
class TP1FANCHEVEN_API AProjectiles : public AActor
{
	GENERATED_BODY()

public:
	AProjectiles();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, Category="Components")
	class USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, Category="Movement")
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	void FireInDirection(const FVector& ShootDir);
};
