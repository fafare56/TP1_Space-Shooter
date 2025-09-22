#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/BoxComponent.h"
#include "Vaisseau.generated.h"

UCLASS()
class TP1FANCHEVEN_API AVaisseau : public APawn
{
	GENERATED_BODY()

public:
	AVaisseau();

protected:
	virtual void BeginPlay() override;

	// Collision et mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Collision")
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* VaisseauMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UFloatingPawnMovement* MovementComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting|Properties")
	class UNiagaraSystem* ExplosionNS;

	// Projetiles
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Projectile")
	TSubclassOf<class AProjectiles> ProjectileClass;

	// Vies
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Vie")
	int32 Vies;

	// Direction actuelle
	FVector CurrentShootDirection;

	int CurrentMoveX;
	int CurrentMoveY;

	// Fonction overlap
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
						UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
						bool bFromSweep, const FHitResult& SweepResult);

	void Shoot();

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Mouvement
	void MoveForward(float Value);
	void MoveRight(float Value);
};
