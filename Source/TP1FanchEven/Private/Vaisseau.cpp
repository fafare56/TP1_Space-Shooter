#include "Vaisseau.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Components/StaticMeshComponent.h"
#include "Meteroite.h"
#include "Projectiles.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "MonGameInstance.h"

AVaisseau::AVaisseau()
{
	PrimaryActorTick.bCanEverTick = true;

	// Mesh
	VaisseauMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VaisseauMesh"));

	// Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	VaisseauMesh->SetupAttachment(CollisionBox);

	CollisionBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_Pawn);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetGenerateOverlapEvents(true);

	// Mouvement
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->UpdatedComponent = RootComponent;

	// Vies
	Vies = 3;
	CurrentMoveX = 0;
	CurrentMoveY = 0;
	CurrentShootDirection = FVector(1,0,0); // par défaut droite
}

void AVaisseau::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC) PC->Possess(this);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVaisseau::OnOverlapBegin);
}

void AVaisseau::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Clamp position dans la zone
	FVector Pos = GetActorLocation();
	Pos.X = FMath::Clamp(Pos.X, -1500.f, -330.f);
	Pos.Y = FMath::Clamp(Pos.Y, -1420.f, 960.f);
	SetActorLocation(Pos);
}

void AVaisseau::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AVaisseau::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVaisseau::MoveRight);
	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AVaisseau::Shoot);
}

void AVaisseau::MoveForward(float Value)
{
	AddMovementInput(FVector::ForwardVector, Value);
	CurrentMoveY = Value;

	if (Value > 0)
	{
		VaisseauMesh->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
		CurrentShootDirection = FVector(0, 1, 0);
	}
	else if (Value < 0)
	{
		VaisseauMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
		CurrentShootDirection = FVector(0, -1, 0);
	}
}

void AVaisseau::MoveRight(float Value)
{
	AddMovementInput(FVector::RightVector, Value);
	CurrentMoveX = Value;

	if (Value > 0)
	{
		VaisseauMesh->SetRelativeRotation(FRotator(0.f, 0.f, 0.f));
		CurrentShootDirection = FVector(1, 0, 0);
	}
	else if (Value < 0)
	{
		VaisseauMesh->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));
		CurrentShootDirection = FVector(-1, 0, 0);
	}
}

void AVaisseau::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
							   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
							   bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && OtherActor->IsA(AMeteroite::StaticClass()))
	{
		OtherActor->Destroy();
		Vies--;
		if (Vies <= 0)
		{
			UMonGameInstance* GI = Cast<UMonGameInstance>(UGameplayStatics::GetGameInstance(this));
			if (GI)
			{
				GI->setMeilleurScore(); 
			}

			Destroy();
			UGameplayStatics::OpenLevel(this, FName("Menu_FIN"));
		}
		if (ExplosionNS)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ExplosionNS,                       
				GetActorLocation(),                
				FRotator::ZeroRotator             
			);
		}
	}
}

void AVaisseau::Shoot()
{
	if (!ProjectileClass) return;

	// Récupère la direction “avant” du mesh
	FVector ShootDirection = VaisseauMesh->GetForwardVector();

	// Applique une rotation de correction si le mesh est décalé de 90 degrés
	FRotator CorrectionRotation = FRotator(0.f, 90.f, 0.f); // ajuste à -90.f si nécessaire
	ShootDirection = CorrectionRotation.RotateVector(ShootDirection);

	// Position de spawn du projectile légèrement devant le vaisseau
	FVector MuzzleLocation = GetActorLocation() + ShootDirection * 100.f;

	// Rotation du projectile pour qu’il regarde vers la direction du tir
	FRotator MuzzleRotation = ShootDirection.Rotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// Spawn du projectile
	AProjectiles* Projectile = GetWorld()->SpawnActor<AProjectiles>(
		ProjectileClass,
		MuzzleLocation,
		MuzzleRotation,
		SpawnParams
	);

	if (Projectile)
	{
		Projectile->FireInDirection(ShootDirection);
	}
}
