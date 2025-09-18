#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "MeteroiteSpawners.generated.h"

class AMeteroite;

UCLASS()
class TP1FANCHEVEN_API AMeteroiteSpawners : public AActor
{
    GENERATED_BODY()
    
public:    
    AMeteroiteSpawners();

protected:
    virtual void BeginPlay() override;

private:
    void SpawnMeteorite();

    FTimerHandle SpawnTimerHandle;

    UPROPERTY(EditAnywhere, Category="Spawning")
    TSubclassOf<AMeteroite> MeteroiteClass;

    UPROPERTY(EditAnywhere, Category="Spawning")
    float SpawnInterval;

    UPROPERTY(EditAnywhere, Category="Spawning")
    float SpawnDistance;

    UPROPERTY(EditAnywhere, Category="Spawning")
    float ScreenHalfWidth;

    UPROPERTY(EditAnywhere, Category="Spawning")
    float ScreenHalfHeight;
};
