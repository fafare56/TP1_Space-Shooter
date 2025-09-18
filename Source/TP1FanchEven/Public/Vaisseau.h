// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Vaisseau.generated.h"

UCLASS()
class TP1FANCHEVEN_API AVaisseau : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVaisseau();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Fonctions de mouvement
	void MoveForward(float Value);
	void MoveRight(float Value);

protected:
	// Composant racine (visuel du vaisseau)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	UStaticMeshComponent* VaisseauMesh;

	// Composant de mouvement
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	class UFloatingPawnMovement* MovementComponent;
};